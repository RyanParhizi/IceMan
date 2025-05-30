#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class Actor : public GraphObject
{
public:
    Actor(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0);
    virtual ~Actor();
    virtual void doSomething() = 0;
    void setAlive(bool alive_);
    bool isAlive();
    
private:
    bool alive = true;
};

class Ice : public Actor {
public:
    Ice(int startX, int startY);
    virtual ~Ice();
    void doSomething() override;

};

class Person : public Actor {
public:
    Person(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0);
    virtual ~Person();
    void annoy(int damage);

protected:
    void processMovementInput(Direction inDir);

private:
    int hitPoints = 0;
    int coordClamp(int&& coord);

};

class Iceman : public Person {
public:
    Iceman(const int& keyRef);
    virtual ~Iceman();
    void doSomething() override;

private:
    const int& key;
    int hitPoints;
    int squirtCount;
    int sonarCount;
    int goldCount;

};

class Goodies : public Actor {

public:
    Goodies(int imageID, int startX, int startY, unsigned int depth = 2)
    : Actor(imageID, startX, startY, right, 1.0, depth)
    {
        state = true;
    }

    virtual ~Goodies() {}

    virtual void doSomething() override = 0;

    bool isVisibleToIceman() const {
        return state;
    }

    void setVisibleToIceman(bool s) {
        state = s;
        setVisible(s);
    }

private:
    bool state;
};

class GoldNugget : public Goodies {
public:
    GoldNugget(int startX, int startY, bool temporary, int lifetime = 100)
    : Goodies(IID_GOLD, startX, startY, 2)
    {
        m_temporary = temporary;
        m_lifetime = lifetime;
        if (!temporary) {
            setVisible(false); 
        } else {
            setVisible(true);
        }
    }    

    virtual ~GoldNugget() {}

    void doSomething() override
    {
        if (!isAlive()) {
            return;
        }

        if (m_temporary) {
            if (--m_lifetime <= 0) {
                setAlive(false);
                return;
            }

            // Add logic to detect if Proteste picks up the gold
            // If so, give score, setAlive(false), play sound
        } else {
            // Add logic to check if Iceman is close enough
            // if so give gold to player, increase score, setAlive(false), play sound
        }
    }

private:
    bool m_temporary;
    int m_lifetime;
};

#endif // ACTOR_H_
