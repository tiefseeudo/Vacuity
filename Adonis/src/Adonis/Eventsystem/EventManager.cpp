#include "pch.h"
#include "EventManager.h"
#include "Adonis/Log.h"

namespace Adonis {

	namespace event {

		std::vector<event_ptr> EventManager::s_queue = std::vector<event_ptr>();
		std::map<Event::evID, std::vector<event_handler>> EventManager::s_subscriptions = std::map<Event::evID, std::vector<event_handler>>();

		bool EventManager::subscribe(size_t id, const event_handler& handler) {
			auto where = s_subscriptions.find(id);

			if (where == s_subscriptions.end()) {
				#ifdef ADONIS_DEBUG
					AD_CORE_TRACE("{0} says Adding i: {1} to subscription map", typeid(EventManager).name(), id);
				#endif
				s_subscriptions[id] = std::vector<event_handler>();
			}
			{
				//Check if handler function already exists
				auto& registered_handlers = s_subscriptions[id];
				for (auto& func : registered_handlers) {
					if (func.target<event_handler>() == handler.target<event_handler>()) {
						//handler function already registered; report failure
						return false;
					}
				}
				#ifdef ADONIS_DEBUG
					AD_CORE_TRACE("{0} says Registered new handler for event with id: {1}", typeid(EventManager).name(), id);
				#endif		
				registered_handlers.push_back(handler);
			}
			return true;
		}

		bool EventManager::unsubscribe(size_t id, const event_handler& handler) {
			auto where = s_subscriptions.find(id);

			if (!(where == s_subscriptions.end())) {
				
				auto& registered_handlers = s_subscriptions[id];
				for (auto it = registered_handlers.begin(); it != registered_handlers.end(); it++) {
					auto& func = *it;
					//if function matches passed handler; erase it and report success
					if (func.target<event_handler>() == handler.target<event_handler>()) {
						registered_handlers.erase(it);
						return true;
					}
				}

			}
			return false;
		}

		void EventManager::queueEvent(event_ptr event) {
			s_queue.push_back(event);
		}

		void EventManager::processEvents() {
			//AD_CORE_INFO("Size of event queue: {0}", s_queue.size());
			auto it = s_queue.begin();
			while (s_queue.size() > 0) {
				auto& event = *it;
				auto subscription = s_subscriptions.find(event->id());
				if (subscription != s_subscriptions.end()) {
					auto& handlers = subscription->second;
					for (auto& handler : handlers) {
						handler(event);
					}
				}
				it = s_queue.erase(it);
				if (s_queue.size() != 0)
					it++;
			}
		}

	}
}