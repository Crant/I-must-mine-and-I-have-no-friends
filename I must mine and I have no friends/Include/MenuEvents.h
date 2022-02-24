#pragma once

#include "Observer.h"

namespace IMM
{
	namespace Events
	{
		class HostButtonPressedEvent : public Event
		{
		public:
			
		};

		class JoinButtonPressedEvent : public Event
		{
		public:

		};
		class InventoryButtonPressedEvent : public Event
		{
		public:
			InventoryButtonPressedEvent(std::shared_ptr<Inventory> cInv)
			{
				this->cInv = cInv;
			};

			std::shared_ptr<Inventory> cInv;
		};
		class InventoryButtonReleasedEvent : public Event
		{
		public:
			InventoryButtonReleasedEvent()
			{
				
			};
			int nOldIndex;
			int nNewIndex;
		};
	}
}