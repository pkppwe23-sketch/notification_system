#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

// ================================
// 1. Decorator Pattern
// ================================

class INotification {
public:
    virtual string getContent() const = 0;
    virtual ~INotification() = default;
};

class SimpleNotification : public INotification {
private:
    string content;

public:
    explicit SimpleNotification(const string& message) : content(message) {}

    string getContent() const override {
        return content;
    }
};

class INotificationDecorator : public INotification {
protected:
    shared_ptr<INotification> notification;

public:
    explicit INotificationDecorator(shared_ptr<INotification> n)
        : notification(n) {}
};

class TimestampDecorator : public INotificationDecorator {
public:
    explicit TimestampDecorator(shared_ptr<INotification> n)
        : INotificationDecorator(n) {}

    string getContent() const override {
        return "[2026-10-21 12:49:32] " + notification->getContent();
    }
};

class SignatureDecorator : public INotificationDecorator {
private:
    string signature;

public:
    SignatureDecorator(shared_ptr<INotification> n, const string& sign)
        : INotificationDecorator(n), signature(sign) {}

    string getContent() const override {
        return notification->getContent() + "\n-- " + signature;
    }
};

// ================================
// 2. Strategy Pattern
// ================================

class INotificationStrategy {
public:
    virtual void send(const string& content) const = 0;
    virtual ~INotificationStrategy() = default;
};

class EmailStrategy : public INotificationStrategy {
public:
    void send(const string& content) const override {
        cout << "[Email] " << content << endl;
    }
};

class SMSStrategy : public INotificationStrategy {
public:
    void send(const string& content) const override {
        cout << "[SMS] " << content << endl;
    }
};

class PopUpStrategy : public INotificationStrategy {
public:
    void send(const string& content) const override {
        cout << "[Popup] " << content << endl;
    }
};

// ================================
// 3. Observer Pattern
// ================================

class IObserver {
public:
    virtual void update(const string& content) = 0;
    virtual ~IObserver() = default;
};

class IObservable {
public:
    virtual void attach(IObserver* observer) = 0;
    virtual void detach(IObserver* observer) = 0;
    virtual void notify(const string& content) = 0;
    virtual ~IObservable() = default;
};

class NotificationObservable : public IObservable {
private:
    vector<IObserver*> observers;

public:
    void attach(IObserver* observer) override {
        if (observer != nullptr) {
            observers.push_back(observer);
        }
    }

    void detach(IObserver* observer) override {
        for (auto it = observers.begin(); it != observers.end(); ++it) {
            if (*it == observer) {
                observers.erase(it);
                return;
            }
        }
    }

    void notify(const string& content) override {
        for (IObserver* observer : observers) {
            if (observer != nullptr) {
                observer->update(content);
            }
        }
    }
};

// ================================
// 4. Notification Engine
// ================================

class NotificationEngine : public IObserver {
private:
    vector<shared_ptr<INotificationStrategy>> strategies;

public:
    void addStrategy(shared_ptr<INotificationStrategy> strategy) {
        if (strategy) {
            strategies.push_back(strategy);
        }
    }

    void update(const string& content) override {
        for (const auto& strategy : strategies) {
            strategy->send(content);
        }
    }
};

class Logger : public IObserver {
public:
    void update(const string& content) override {
        cout << "[Logger] Notification received: " << content << endl;
    }
};

// ================================
// 5. Singleton Notification Service
// ================================

class NotificationService {
private:
    NotificationObservable observable;

    NotificationService() = default;
    NotificationService(const NotificationService&) = delete;
    NotificationService& operator=(const NotificationService&) = delete;

public:
    static NotificationService& getInstance() {
        static NotificationService instance;
        return instance;
    }

    void attach(IObserver* observer) {
        observable.attach(observer);
    }

    void detach(IObserver* observer) {
        observable.detach(observer);
    }

    void sendNotification(const shared_ptr<INotification>& notification) {
        if (notification) {
            observable.notify(notification->getContent());
        }
    }
};

// ================================
// 6. Main Function
// ================================

int main() {
    NotificationEngine engine;
    Logger logger;

    engine.addStrategy(make_shared<EmailStrategy>());
    engine.addStrategy(make_shared<SMSStrategy>());
    engine.addStrategy(make_shared<PopUpStrategy>());

    NotificationService& service = NotificationService::getInstance();
    service.attach(&engine);
    service.attach(&logger);

    shared_ptr<INotification> notification =
        make_shared<SimpleNotification>("Your order has been shipped!");

    notification = make_shared<TimestampDecorator>(notification);
    notification = make_shared<SignatureDecorator>(notification, "Customer Care");

    service.sendNotification(notification);

    service.detach(&engine);
    service.detach(&logger);

    return 0;
}
