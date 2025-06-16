#include <queue>
#include <vector>
#include <utility>
#include <algorithm>
#include "StudentWorld.h"
#include "Actor.h"
#include <sstream>
#include <iomanip>
#include <queue>

GameWorld* createStudentWorld(std::string assetDir)
{
	return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(std::string assetDir)
	: GameWorld(assetDir) {
}

StudentWorld::~StudentWorld() {
	cleanUp();
}

int StudentWorld::init() {
	// Create the player's IceMan
	player = new IceMan(this, 30, 60);
	
	// Generate Ice Grid
	iceGridAction(1);
	
	// Set random seed for various functions
	std::srand(static_cast<unsigned int>(time(nullptr)));

	// Generate Level Actors
	generateLevelActors();
	
	// Calculate target number of protestors for the current level
	targetProtestorCount = std::min(15, int(2 + currentLevel * 1.5));
	
	// Begin game loop
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
	// Have the player's IceMan do something
	player->doSomething();

	// Have all the actors in the level do something
	std::for_each(levelActors.begin(), levelActors.end(),
		[](Actor* actor) { actor->doSomething(); });

	// Remove all the dead actors in the level
	levelActors.erase(std::remove_if(levelActors.begin(), levelActors.end(),
		[](Actor* actor) { if (!actor->isAlive()) { delete actor; return true; } return false;}), levelActors.end());

	// Remove all the ice underneath the player
	clearIce(player->getX(), player->getY());

	// Add new actors to the level according to ambient spawn mechanics
	addNewActors();

	// Update display text at the top of the screen
	updateDisplayText();

	// Win Condition
	if (m_oilBarrelsLeft <= 0) {
		currentLevel++;
		playSound(SOUND_FINISHED_LEVEL);
		return GWSTATUS_FINISHED_LEVEL;
	}

	// Lose Condition
	if (player->getHitPoints() <= 0) { // Lose Condition
		livesLeft--;
		decLives(); // Completely forgot these functions were inhereted. Most of the inhereted functionality was recreated in this class.
		playSound(SOUND_PLAYER_GIVE_UP);
		return GWSTATUS_PLAYER_DIED;
	}

	// Continue the game loop
	return GWSTATUS_CONTINUE_GAME;
}
void StudentWorld::cleanUp() {
	// Deallocate the player's IceMan
	delete player;
	player = nullptr;

	// Deallocate the ice grid
	iceGridAction(0);

	// Deallocate all actors in the level
	for (Actor* actor : levelActors) {
		delete actor;
		actor = nullptr;
	}
	levelActors.clear();

	// Protester timer prep for next level gen
	protesterSpawnTimer = 0;
}

// Add an actor to the world.
void StudentWorld::addActor(Actor* a) {
	
	levelActors.push_back(a);
}

// Clear a 4x4 region of Ice.
void StudentWorld::clearIce(int x, int y) {
	
	bool icePresent = false;
	
	// Delete ice in a 4x4 grid relative to the given coords
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (iceGrid[x + i][y + j]){
				icePresent = true;
			}
			delete iceGrid[x + i][y + j];
			iceGrid[x + i][y + j] = nullptr;
		}
	}

	// Play sound if ice is destroied
	if (icePresent) {
		playSound(SOUND_DIG);
	}
}

// Can actor move to x,y?
bool StudentWorld::canActorMoveTo(Actor* a, int x, int y) const {
	
	// Out of bounds condition
	if (x < 0 || 60 < x || y < 0 || 60 < y) {
		return false;
	}

	// For all actors that cannot be passed through, stop the moving actor
	for (Actor* actor : levelActors) {
		if (!actor->canActorsPassThroughMe() && actor != a) {
			if (x - 4 < actor->getX() && actor->getX() < x + 4 &&
				y - 4 < actor->getY() && actor->getY() < y + 4) {
				return false;
			}
		}
	}
	
	// If the moving actor is not the player, any ice stops the actor
	if (a != player) {
		if (!allIceAtCoords(x, y, false)) {
			return false;
		}
	}
	return true;
}

// Annoy all other actors within radius of annoyer, returning the number of actors annoyed.
int StudentWorld::annoyAllNearbyActors(Actor* annoyer, int points, int radius) {
	int annoiedActorCount = 0;
	
	for (Actor* actor : levelActors) {

		// Actors cannot hurt themselves
		if (actor == annoyer) {
			continue;
		}

		// An actor is in annoyer's radious
		if (std::hypot(annoyer->getX() - actor->getX(), annoyer->getY() - actor->getY()) <= radius) {
			actor->annoy(points);
			annoiedActorCount++;
		}
	}

	// If the annoyer can hurt the ice man (not water squirt)
	if(annoyer->canHurtIceMan()) {
		if (std::hypot(annoyer->getX() - player->getX(), annoyer->getY() - player->getY()) <= radius) {
			player->annoy(points);
			annoiedActorCount++;
		}
	}

	return annoiedActorCount;
}

// Reveal all objects within radius of x,y.
void StudentWorld::revealAllNearbyObjects(int x, int y, int radius) {
	for (Actor* a : levelActors) {
		if (std::hypot(a->getX() - x, a->getY() - y) <= 12) {
			a->setVisible(true);
		}
	}
}

// If the IceMan is within radius of a, return a pointer to the
// IceMan, otherwise null.
Actor* StudentWorld::findNearbyIceMan(Actor* a, int radius) const {
    
	// Get distance from actor to player
	double distance = std::hypot(a->getX() - player->getX(), a->getY() - player->getY());
    
	if (distance <= radius) {
        return player;
    }
    
	return nullptr;
}

// If at least one actor that can pick things up is within radius of a,
// return a pointer to one of them, otherwise null.
Actor* StudentWorld::findNearbyPickerUpper(Actor* a, int radius) const {

    for (Actor* actor : levelActors) {
        if (actor->canPickThingsUp() &&
            std::hypot(a->getX() - actor->getX(), a->getY() - actor->getY()) <= radius) {
            return actor;
        }
    }
    return nullptr;
}

// Annoy the IceMan.
void StudentWorld::annoyIceMan() {
	// NOT YET IMPLEMENTED
}

// Give IceMan some sonar charges.
void StudentWorld::giveIceManSonar() {
	// NOT YET IMPLEMENTED
}

// Give IceMan some water.
void StudentWorld::giveIceManWater() {
	// NOT YET IMPLEMENTED
}

// Is the Actor a facing toward the IceMan?
bool StudentWorld::facingTowardIceMan(Actor* a) const {
	// NOT YET IMPLEMENTED
	return false;
}

// If the Actor a has a clear line of sight to the IceMan, return
// the direction to the IceMan, otherwise GraphObject::none.
GraphObject::Direction StudentWorld::lineOfSightToIceMan(Actor* a) const {

	GraphObject::Direction relativeDirection = GraphObject::none;

	if (a->getX() == player->getX()) {
		if (a->getY() < player->getY()) {
			relativeDirection = GraphObject::up;
		}
		else if (a->getY() > player->getY()) {
			relativeDirection = GraphObject::down;
		}
	}
	else if (a->getY() == player->getY()) {
		if (a->getX() < player->getX()) {
			relativeDirection = GraphObject::right;
		}
		else if (a->getX() > player->getX()) {
			relativeDirection = GraphObject::left;
		}
	}

	switch (relativeDirection) {
	case GraphObject::none:
		return GraphObject::none;
		break;
	case GraphObject::up:
		for (int i = a->getY(); i <= player->getY(); i++) {
			if (!canActorMoveTo(nullptr, a->getX(), i)) {
				return GraphObject::none;
			}
		}
		return GraphObject::up;
		break;
	case GraphObject::down:
		for (int i = a->getY(); i >= player->getY(); i--) {
			if (!canActorMoveTo(nullptr, a->getX(), i)) {
				return GraphObject::none;
			}
		}
		return GraphObject::down;
		break;
	case GraphObject::left:
		for (int i = a->getX(); i >= player->getX(); i--) {
			if (!canActorMoveTo(nullptr, i, a->getY())) {
				return GraphObject::none;
			}
		}
		return GraphObject::left;
		break;
	case GraphObject::right:
		for (int i = a->getX(); i <= player->getX(); i++) {
			if (!canActorMoveTo(nullptr, i, a->getY())) {
				return GraphObject::none;
			}
		}
		return GraphObject::right;
		break;
	}
}

// Return whether the Actor a is within radius of IceMan.
bool StudentWorld::isNearIceMan(Actor* a, int radius) const {
	// NOT YET IMPLEMENTED
	return false;
}

// Determine the direction of the first move a quitting protester
// makes to leave the oil field.
GraphObject::Direction StudentWorld::determineFirstMoveToExit(int x, int y) {
	return pathFind(x, y, 60, 60, NULL);
}

// Determine the direction of the first move a hardcore protester
// makes to approach the IceMan.
GraphObject::Direction StudentWorld::determineFirstMoveToIceMan(int x, int y, int depthLimit) {
	// NOT YET IMPLEMENTED
	return pathFind(x, y, player->getX(), player->getY(), depthLimit);
}

// Depending on the arguments, this function will either return
// true to a 4x4 area being filled with ice or empty of ice, or
// false if it's a mix of ice and no ice
bool StudentWorld::allIceAtCoords(int x, int y, bool mode) const {

	// Flags for return value
	bool allIce = true;
	bool noIce = true;

	// Check for ice and update flags
	for (int i = x; i < x + 4; i++) {
		for (int j = y; j < y + 4; j++) {
			if (iceGrid[i][j]) {
				noIce = false;
			}
			if (!iceGrid[i][j]) {
				allIce = false;
			}
		}
	}

	// If mode is true, will return true if area is filled with ice
	if (mode == true) { return allIce; }

	// If mode is false, will return true if area is empty of ice
	if (mode == false) { return noIce; }

	// Return false for a mix of ice and no ice in the area.
	return false;
}


// Depending on the argument, this function will either create or destroy the ice grid.
void StudentWorld::iceGridAction(bool option) {
	
	// For each column of ice
	int colN = 0;
	for (auto& iceCol : iceGrid) {

		// For each row of ice (for each ice block in a column)
		int rowN = 0;
		for (auto& iceBlock : iceCol) {

			// Ice grid shape condtion
			if ((colN < 30 || colN > 33 || rowN < 4) && rowN < 60) {

				// Option true generates ice grid
				if (option) {
					iceBlock = new Ice(this, colN, rowN);
				}
				// Option false desrtoys ice grid
				else {
					delete iceBlock;
					iceBlock = nullptr;
				}
			}
			rowN++;
		}
		colN++;
	}
}

// This generates all the actors present at the start of a level
void StudentWorld::generateLevelActors() {

	// Calculate needed initial actors for the given level
	int boulderCount = std::min(currentLevel / 2 + 2, 9);
	int goldCount = std::max(5 - currentLevel / 2, 2);
	int barrelCount = std::min(2 + currentLevel, 21);

	// Generate boulders // These segments share similar code. Consider compressing into single template function.
	for (int i = 0; i < boulderCount; i++) {
		std::pair<int, int> newLocation = findNewLocation(0, 20, 60, 56, true);
		if (newLocation.first == -1 && newLocation.second == -1) { break; } // Bad coords, no space, skip generation
		addActor(new Boulder(this, newLocation.first, newLocation.second));
		clearIce(newLocation.first, newLocation.second);
	}

	// Generate gold nuggets
	for (int i = 0; i < goldCount; i++) {
		std::pair<int, int> newLocation = findNewLocation(0, 0, 60, 60, true);
		if (newLocation.first == -1 && newLocation.second == -1) { break; } // Bad coords, no space, skip generation
        addActor(new GoldNugget(this, newLocation.first, newLocation.second,
                 false,    // temporary? (false = permanent)
                 true,     // pickupByIceMan? (true = Iceman can pick it up)
                 true));
	}

	// Generate oil barrels
	for (int i = 0; i < barrelCount; i++) {
		std::pair<int, int> newLocation = findNewLocation(0, 0, 60, 60, true);
		if (newLocation.first == -1 && newLocation.second == -1) { break; } // Bad coords, no space, skip generation
		addActor(new OilBarrel(this, newLocation.first, newLocation.second));
	}
}

std::pair<int, int> StudentWorld::findNewLocation(int x1, int y1, int x2, int y2, bool underIce) { // Under the ice for starting actors
	
	// Only try generating new positions so many times (in case there is no more room left to avoid infinite loop)
	for(int loops = 0; loops < 3600; loops++) {

		// Generate potential location
		int newX = std::rand() % (x2 - x1 + 1) + x1;
		int newY = std::rand() % (y2 - y1 + 1) + y1;

		// Check for are full of ice
		if (allIceAtCoords(newX, newY, underIce)) {

			// If new location is not under the ice (water pool), there are no further restructions 
			if (!underIce) {
				return std::pair<int, int>(newX, newY);
			}
			// Otherwise check spacing

			// For all actors, check proximity and update 'too close' flag if necessry
			bool tooClose = false;
			for (Actor* actor : levelActors) {
				if (std::hypot(newX - actor->getX(), newY - actor->getY()) < 6) {
					tooClose = true;
				}
			}

			// If potential location is NOT too close to any existing actor, or
			// return that new location
			if (!tooClose) {
				return std::pair<int, int>(newX, newY);
			}
			// Otherwise, try again
		}
	}

	// If this fails, cerr and return bad coords
	std::cerr << "findNewLocation() has failed. Level is full after 3600 spawn attempts.";
	return std::pair<int, int>(-1, -1);
}

// Will return true depending on passed in decimal chance
bool StudentWorld::getTrueByChance(double chance) {
	return (static_cast<double>(rand()) / RAND_MAX) < chance;
}

// This set of functions can probably be inlined

void StudentWorld::createHardcoreProtestor() {
	addActor(new HardcoreProtester(this, 60, 60, IID_HARD_CORE_PROTESTER));
}

void StudentWorld::createRegularProtestor() {
	addActor(new RegularProtester(this, 60, 60, IID_PROTESTER));
}

void StudentWorld::createSonarKit() {
	addActor(new SonarKit(this, 0, 60, currentLevel));
}

void StudentWorld::createWaterPool() {
	std::pair<int, int> newLocation = findNewLocation(0, 0, 60, 60, false);
	addActor(new WaterPool(this, newLocation.first, newLocation.second));
}

// Add new actors to the level according to ambient spawn mechanics
void StudentWorld::addNewActors() {
	
	// Creating a protester
	if (protesterSpawnTimer <= 0) {
		if (currentProtesterCount < targetProtestorCount) {

			// Chance of creating hardcore protestor
			if (getTrueByChance(std::min(90., currentLevel * 10. + 30)/100)) {
				addActor(new HardcoreProtester(this, 60, 60, IID_HARD_CORE_PROTESTER));
			}

			// Chance of creating regular protester
			else {
				addActor(new RegularProtester(this, 60, 60, IID_PROTESTER));
			}
		}
		protesterSpawnTimer = std::max(25, 200 - currentLevel);
	}
	else {
		protesterSpawnTimer--;
	}

	// Creating a goodie
	if (getTrueByChance(1. / (currentLevel * 30 + 290))) {
		
		// Chance of creating a sonar kit
		if (getTrueByChance(1. / 5)) {
			addActor(new SonarKit(this, 0, 60, currentLevel));
		}

		// Chance of creating a water pool
		else {
			std::pair<int, int> newLocation = findNewLocation(0, 0, 60, 60, false);
			addActor(new WaterPool(this, newLocation.first, newLocation.second));
		}
	}


}

// Update display text at the top of the screen
void StudentWorld::updateDisplayText() {

	// Create string stream
	std::ostringstream displayText;

	// Build string stream
	using namespace std;
	displayText << "Lvl: " << right << setw(2) << currentLevel << "  "
		<< "Lives: " << setw(1) << livesLeft << "  "
		<< "Hlth: " << setw(3) << 100 * player->getHitPoints() / player->getStartingHitPoints() << "%  "
		<< "Wtr: " << setw(2) << player->getWater() << "  "
		<< "Gld: " << setw(2) << player->getGold() << "  "
		<< "Oil Left: " << setw(2) << m_oilBarrelsLeft << "  "
		<< "Sonar: " << setw(2) << player->getSonar() << "  "
		<< "Scr: " << setw(6) << setfill('0') << currentScore;

	// Pass string stream to inherited function for update
	setGameStatText(displayText.str());
}

// Adds amount to current score
void StudentWorld::addToScore(int amount) {
	currentScore += amount;
	increaseScore(amount); // Completely forgot these functions were inhereted. Most of the inhereted functionality was recreated in this class.
} // ^ ERROR: This isn't working and the GameWorld's currentScore is stuck at 0. StudentWorld's currentScore is fine.

// Returns the first movement direction to get from (x1, y2) to (x2, y2)
GraphObject::Direction StudentWorld::pathFind(int x1, int y1, int x2, int y2, int depthLimit)
{
	if (x1 == x2 && y1 == y2) {
		return GraphObject::none;
	}

	int dx[] = { 0, 0, 0, -1, 1 };
	int dy[] = { 0, 1, -1, 0, 0 };

	bool visited[61][61] = { false };
	std::queue<std::tuple<int, int, GraphObject::Direction, int>> searchQueue;
	
	visited[x1][y1] = true;
	
	// Begin search paths from surrounding coords and store starting direction
							// Up				  // Right			// -> Down -> Left ->	
	for (int startingDirection = 1; startingDirection <= 4; startingDirection++) {
		int pathStartX = x1 + dx[startingDirection];
		int pathStartY = y1 + dy[startingDirection];

		if (canActorMoveTo(nullptr, pathStartX, pathStartY)) {
			visited[pathStartX][pathStartY] = true;
			searchQueue.push({ pathStartX, pathStartY, GraphObject::Direction(startingDirection), 1 });
		}
	}
	
while (!searchQueue.empty()) {
		int x = std::get<0>(searchQueue.front());
		int y = std::get<1>(searchQueue.front());
		GraphObject::Direction startingDirection = std::get<2>(searchQueue.front());
		int depth = std::get<3>(searchQueue.front());

		if (startingDirection == GraphObject::right) {
			int* c = nullptr;
		}

		// End found, return starting direction
		if (x == x2 && y == y2) {
			return startingDirection;
		}

		for (int nextDirection = 1; nextDirection <= 4; nextDirection++) {
			int pathNextX = x + dx[nextDirection];
			int pathNextY = y + dy[nextDirection];
			
			if (visited[pathNextX][pathNextY])
			{
				visited;
				pathNextX;
				pathNextY;
				int* b = nullptr;

			}

			if (canActorMoveTo(nullptr, pathNextX, pathNextY) && !visited[pathNextX][pathNextY]) {
				visited[pathNextX][pathNextY] = true;
				searchQueue.push({ pathNextX, pathNextY, GraphObject::Direction(startingDirection), depth++ });
			}
		}

		searchQueue.pop();

		if (depthLimit != NULL) {
			if (depth >= depthLimit) {
				break;
			}
		}
	}
	return GraphObject::none;
}

void StudentWorld::getIceManLocation(int& x, int& y) const {
    if (player) {
        x = player->getX();
        y = player->getY();
    }
}

// Helper for BFS
struct Node {
    int x, y;
    int steps;
    GraphObject::Direction firstDir;
};

int StudentWorld::stepsToTarget(int startX, int startY, int endX, int endY) const {
    const int WIDTH = 64, HEIGHT = 64; // adjust as needed
    bool visited[WIDTH][HEIGHT] = {false};
    std::queue<Node> q;
    q.push({startX, startY, 0, GraphObject::none});
    visited[startX][startY] = true;

    while (!q.empty()) {
        Node curr = q.front(); q.pop();
        if (curr.x == endX && curr.y == endY)
            return curr.steps;

        const int dx[4] = { -1, 1, 0, 0 };
        const int dy[4] = { 0, 0, 1, -1 };
        for (int dir = 0; dir < 4; ++dir) {
            int nx = curr.x + dx[dir], ny = curr.y + dy[dir];
            if (nx < 0 || nx >= WIDTH || ny < 0 || ny >= HEIGHT) continue;
            if (visited[nx][ny]) continue;
            if (!canActorMoveTo(nullptr, nx, ny)) continue;
            visited[nx][ny] = true;
            q.push({nx, ny, curr.steps + 1, curr.steps == 0 ? static_cast<GraphObject::Direction>(dir) : curr.firstDir});
        }
    }
    return -1;
}

GraphObject::Direction StudentWorld::determineFirstMoveToTarget(int startX, int startY, int endX, int endY) const {
    const int WIDTH = 64, HEIGHT = 64; // adjust as needed
    bool visited[WIDTH][HEIGHT] = {false};
    std::queue<Node> q;
    q.push({startX, startY, 0, GraphObject::none});
    visited[startX][startY] = true;

    while (!q.empty()) {
        Node curr = q.front(); q.pop();
        if (curr.x == endX && curr.y == endY)
            return curr.firstDir;

        const int dx[4] = { -1, 1, 0, 0 };
        const int dy[4] = { 0, 0, 1, -1 };
        for (int dir = 0; dir < 4; ++dir) {
            int nx = curr.x + dx[dir], ny = curr.y + dy[dir];
            if (nx < 0 || nx >= WIDTH || ny < 0 || ny >= HEIGHT) continue;
            if (visited[nx][ny]) continue;
            if (!canActorMoveTo(nullptr, nx, ny)) continue;
            visited[nx][ny] = true;
            GraphObject::Direction firstDir = (curr.steps == 0) ? static_cast<GraphObject::Direction>(dir) : curr.firstDir;
            q.push({nx, ny, curr.steps + 1, firstDir});
        }
    }
    return GraphObject::none;
}