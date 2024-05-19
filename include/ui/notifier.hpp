#pragma once

#include <functional>
#include <map>

namespace cpl_ui
{
    template<typename... Args>
    class notifier
    {
    public:

        // Subscriber ID type
        typedef size_t subscriber_id_t;
        // Callback signature for notifications
        typedef std::function<void (Args...)> on_notify_t;

        /**
         * @brief Notify subscribers that an event has occurred.
         * 
         * @param args Notification Arguments.
         */
        void notify(Args... args)
        {
            for(auto& subscriber : m_subscribers)
            {
                subscriber.second(args...);
            }
        }

        /**
         * @brief Subscribe to notification events.
         * 
         * @param callback Event callback
         * @return subscriber_id_t   Subscriber ID
         */
        subscriber_id_t subscribe(on_notify_t callback)
        {
            subscriber_id_t next = m_next_subscriber_id++;
            m_subscribers[next] = callback;
            return next;
        }

        /**
         * @brief Unsubscribe from callback events.
         * 
         * @param subscriber_id ID associated with a subscription to be cancelled.
         */
        void unsubscribe(subscriber_id_t subscriber_id)
        {
            m_subscribers.erase(subscriber_id);
        }

    private:

        /// @brief  Next Subscriber ID
        subscriber_id_t                         m_next_subscriber_id = 0;
        /// @brief Subscribers
        std::map<subscriber_id_t, on_notify_t>  m_subscribers;
    };
}