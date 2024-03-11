
/***
 * @file  Modules/c_topic_subscriber.hpp
 * @brief Publish-subscribe pattern implementation
 */

#pragma once

#ifndef EC_LIB_MODULES_PUBSUB_HPP
#define EC_LIB_MODULES_PUBSUB_HPP

#include <memory>
#include <vector>
#include <algorithm>

namespace modules::pub_sub {

/***
 * @brief Base class for subscriber
 * @note This class is pure virtual, its inheritance class must implement the publisherCallback function
 * @tparam MessageType The type of message in the publish-subscribe pattern
 */
template<typename MessageType>
class SubscriberBase {
 public:
  /***
   * @brief Callback function for topic of this subscriber
   * @param message Message from the topic
   * @note This pure virtual function must be implemented by the inheritance class
   * @note to handle the incoming message on its need
   */
  virtual void publisherCallback(const MessageType &message) = 0;
};

/***
 * @brief Base class for publisher
 * @tparam MessageType The type of message in the publish-subscribe pattern
 */
template<typename MessageType>
class PublisherBase {
 public:
  void registerSubscriber(SubscriberBase<MessageType> &subscriber);
  void unregisterSubscriber(SubscriberBase<MessageType> &subscriber);
 protected:
  void advertise(const MessageType &message);

  // Be aware that the subscriber objects are stored as raw pointers
  std::vector<SubscriberBase<MessageType> *> _subscribers;
};

}   // namespace Modules::PubSub



/**********************/
/*** Implementation ***/
/**********************/

/***
 * @brief  Register a subscriber to this topic
 * @tparam MessageType The type of message in the topic-subscriber pattern
 * @param  subscriber Subscriber object to be registered
 */
template<typename MessageType>
void modules::pub_sub::PublisherBase<MessageType>::registerSubscriber(SubscriberBase<MessageType> &subscriber) {
  if (std::find(this->_subscribers.begin(), this->_subscribers.end(), &subscriber) == this->_subscribers.end()) {
    this->_subscribers.push_back(&subscriber);
  }
}

/***
 * @brief  Unregister a subscriber from this topic
 * @tparam MessageType The type of message in the topic-subscriber pattern
 * @param  subscriber Subscriber object to be unregistered
 */
template<typename MessageType>
void modules::pub_sub::PublisherBase<MessageType>::unregisterSubscriber(SubscriberBase<MessageType> &subscriber) {
  auto it = std::find(this->_subscribers.begin(), this->_subscribers.end(), &subscriber);
  if (it != this->_subscribers.end()) {
    this->_subscribers.erase(it);
  }
}

/***
 * @brief  Publish a message to this topic
 * @tparam MessageType The type of message in the topic-subscriber pattern
 * @param  message Message to be published
 */
template<typename MessageType>
void modules::pub_sub::PublisherBase<MessageType>::advertise(const MessageType &message) {
  for (const auto &subscriber : this->_subscribers) {
    subscriber->publisherCallback(message);
  }
}

#endif // EC_LIB_MODULES_PUBSUB_HPP

/* EOF */
