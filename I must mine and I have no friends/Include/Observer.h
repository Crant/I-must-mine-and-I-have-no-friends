#pragma once 

#include <set>
#include <map>


enum class EventType : unsigned char
{
	HostButtonPressedEvent,
	JoinButtonPressedEvent,
	WorldCreated,
	TilePlaced,
	TileRemoved

};
class Event
{
public:
	EventType type;
	virtual ~Event() {}
};

class Observer
{
protected:
	virtual void OnEvent(Event*) {};
	virtual ~Observer() {};

	friend class Observed;
};

class Observed
{
private:
	std::set<Observer*> zObservers;
	std::map<Observer*, int> zNewObservers;
	bool zInUse;

public:
	inline void AddObserver(Observer* observer)
	{
		if (observer)
		{
			if (zInUse)
			{
				zNewObservers[observer]++;
			}
			else
			{
				zObservers.insert(observer);
			}
		}
	}

	inline void RemoveObserver(Observer* observer)
	{
		if (observer)
		{
			if (zInUse)
			{
				zNewObservers[observer]--;
			}
			else
			{
				zObservers.erase(observer);
			}
		}
	}

protected:
	Observed(Observer* def = 0) :
		zInUse(false)
	{
		AddObserver(def);
	}

	virtual ~Observed()
	{
	}

    inline void NotifyObservers(Event* e)
	{
		if (zObservers.size())
		{
			zInUse = true;

			// Notify Observers
			for (std::set<Observer*>::iterator i = zObservers.cbegin(); i != zObservers.cend(); ++i)
			{
				auto i2 = zNewObservers.find(*i);
				if (i2 == zNewObservers.cend() || i2->second > 0)
				{
					try
					{
						(*i)->OnEvent(e);
					}
					catch (...)
					{

					}
				}
			}

			// New Observers
			for (auto i = zNewObservers.cbegin(); i != zNewObservers.cend(); ++i)
			{
				if (i->second < 0)
				{
					zObservers.erase(i->first);
				}
				else if (i->second > 0)
				{
					zObservers.insert(i->first);
				}
			}
			zNewObservers.clear();

			zInUse = false;
		}
	}
};