/**
 * @file  modules/pubsub.hpp
 * @brief 模仿ROS的发布-订阅消息模式，但是稍有不同
 * @brief 去掉了Topic的概念，只保留了Publisher和Subscriber
 */

#ifndef EC_LIB_MODULES_PUBSUB_HPP
#define EC_LIB_MODULES_PUBSUB_HPP

#include <algorithm>
#include <memory>
#include <vector>

namespace irobot_ec::modules {

/**
 * @brief 订阅者的基类
 * @note  用于实现发布-订阅消息模式，订阅者需要继承这个类并实现RxCallback()函数
 * @tparam MessageType 消息类型
 */
template <typename MessageType>
class SubscriberBase {
 public:
  /**
   * @brief 处理接收到的消息的回调函数
   * @param message 订阅者接收到的消息
   * @attention 这是一个纯虚函数，必须在子类中实现
   */
  virtual void RxCallback(const MessageType &message) = 0;
};

/**
 * @brief 发布者的基类
 * @tparam MessageType 消息类型
 */
template <typename MessageType>
class PublisherBase {
 public:
  void RegisterSubscriber(SubscriberBase<MessageType> &subscriber);
  void UnregisterSubscriber(SubscriberBase<MessageType> &subscriber);

 protected:
  void Publish(const MessageType &message);

  // Be aware that the subscriber objects are stored as raw pointers
  std::vector<SubscriberBase<MessageType> *> subscribers_;
};

}  // namespace irobot_ec::modules

/*********************/
/** Implementation ***/
/*********************/

/**
 * @brief  在这个发布者的订阅者列表中注册一个订阅者
 * @tparam MessageType 消息类型
 * @param  subscriber  订阅者对象
 */
template <typename MessageType>
void irobot_ec::modules::pub_sub::PublisherBase<MessageType>::RegisterSubscriber(
    SubscriberBase<MessageType> &subscriber) {
  if (std::find(this->subscribers_.begin(), this->subscribers_.end(), &subscriber) == this->subscribers_.end()) {
    this->subscribers_.push_back(&subscriber);
  }
}

/**
 * @brief  从这个发布者的订阅者列表中注销一个订阅者
 * @tparam MessageType 消息类型
 * @param  subscriber  订阅者对象
 */
template <typename MessageType>
void irobot_ec::modules::pub_sub::PublisherBase<MessageType>::UnregisterSubscriber(
    SubscriberBase<MessageType> &subscriber) {
  auto it = std::find(this->subscribers_.begin(), this->subscribers_.end(), &subscriber);
  if (it != this->subscribers_.end()) {
    this->subscribers_.erase(it);
  }
}

/**
 * @brief  发布消息
 * @tparam MessageType 消息类型
 * @param  message     消息对象
 */
template <typename MessageType>
void irobot_ec::modules::pub_sub::PublisherBase<MessageType>::Publish(const MessageType &message) {
  for (const auto &subscriber : this->subscribers_) {
    subscriber->RxCallback(message);
  }
}

#endif  // EC_LIB_MODULES_PUBSUB_HPP

/* EOF */
