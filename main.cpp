#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

// ==========================================
// 1. Decorator Pattern Components
// ==========================================

// Abstract Component
class INotification {
public:
    virtual string getContent() const = 0;
    virtual ~INotification() {}
};

// Concrete Component: simple text notification
class SimpleNotification : public INotification {
private:
    string text;
public:
    SimpleNotification(const string& msg) {
        text = msg;
    }
    string getContent() const override {
        return text;
    }
};

// Abstract Decorator: wraps a Notification object
class INotificationDecorator : public INotification {
protected:
    INotification* notification;
public:
    INotificationDecorator(INotification* n) {
        notification = n;
    }
    virtual ~INotificationDecorator() {
        delete notification; // Cascading deletion of wrapped components
    }
};

// Concrete Decorator 1: adds a timestamp
class TimestampDecorator : public INotificationDecorator {
public:
    TimestampDecorator(INotification* n) : INotificationDecorator(n) {}
    string getContent() const override {
        return "[2026-10-21 12:49:32] " + notification->getContent();
    }
};

// Concrete Decorator 2: appends a signature
class SignatureDecorator : public INotificationDecorator {
private:
    string signature;
public:
    SignatureDecorator(INotification* n, const string& sig) : INotificationDecorator(n) {
        signature = sig;
    }
    string getContent() const override {
        return notification->getContent() + "\n-- " + signature + "\n";
    }
};


// ==========================================
// 2. Observer Pattern Interfaces
// ==========================================

class IObserver {
public:
    virtual void update() = 0;
    virtual ~IObserver() {}
};

class IObservable {
public:
    virtual void addObserver(IObserver* observer) = 0;
    virtual void removeObserver(IObserver* observer) = 0;
    virtual void notifyObservers() = 0;
    virtual ~IObservable() {}
};

// Concrete Observable
class NotificationObservable : public IObservable {
private:
    vector<IObserver*> observers;
    INotification* currentNotification;
public:
    NotificationObservable() {
        currentNotification = nullptr;
    }
    
    ~NotificationObservable() {
        if (currentNotification != nullptr) {
            delete currentNotification;
        }
    }

    void addObserver(IObserver* obs) override {
        observers.push_back(obs);
    }

    void removeObserver(IObserver* obs) override {
        observers.erase(remove(observers.begin(), observers.end(), obs), observers.end());
    }

    void notifyObservers() override {
        for (unsigned int i = 0; i < observers.size(); i++) {
            observers[i]->update();
        }
    }

    void setNotification(INotification* notification) {
        if (currentNotification != nullptr) {
            delete currentNotification;
        }
        currentNotification = notification;
        notifyObservers();
    }

    INotification* getNotification() {
        return currentNotification;
    }

    string getNotificationContent() {
        if (currentNotification != nullptr) {
            return currentNotification->getContent();
        }
        return "";
    }
};


// ==========================================
// 3. Strategy Pattern Components
// ==========================================

// Strategy Interface
class INotificationStrategy {
public:
    virtual void sendNotification(string content) = 0;
    virtual ~INotificationStrategy() {}
};

// Concrete Strategy 1: Email
class EmailStrategy : public INotificationStrategy {
private:
    string emailId;
public:
    EmailStrategy(string emailId) {
        this->emailId = emailId;
    }
    void sendNotification(string content) override {
        cout << "Sending email Notification to: " << emailId << "\n" << content << "\n";
    }
};

// Concrete Strategy 2: SMS
class SMSStrategy : public INotificationStrategy {
private:
    string mobileNumber;
public:
    SMSStrategy(string mobileNumber) {
        this->mobileNumber = mobileNumber;
    }
    void sendNotification(string content) override {
        cout << "Sending SMS Notification to: " << mobileNumber << "\n" << content << "\n";
    }
};

// Concrete Strategy 3: PopUp
class PopUpStrategy : public INotificationStrategy {
public:
    void sendNotification(string content) override {
        cout << "Sending Popup Notification: \n" << content << "\n";
    }
};


// ==========================================
// 4. Multi-Strategy Concrete Observer
// ==========================================

class NotificationEngine : public IObserver {
private:
    NotificationObservable* notificationObservable;
    vector<INotificationStrategy*> notificationStrategies;
public:
    NotificationEngine(NotificationObservable* observable) {
        this->notificationObservable = observable;
    }

    // Dynamic clean-up of dynamically registered strategies
    ~NotificationEngine() {
        for (auto strategy : notificationStrategies) {
            delete strategy;
        }
    }

    void addNotificationStrategy(INotificationStrategy* ns) {
        this->notificationStrategies.push_back(ns);
    }

    void update() override {
        string notificationContent = notificationObservable->getNotificationContent();
        for (const auto strategy : notificationStrategies) {
            strategy->sendNotification(notificationContent);
        }
    }
};

// Concrete Observer 2: Logger
class Logger : public IObserver {
private:
    NotificationObservable* notificationObservable;
public:
    Logger(NotificationObservable* observable) {
        this->notificationObservable = observable;
    }

    void update() override {
        cout << "Logging New Notification : \n" << notificationObservable->getNotificationContent() << "\n";
    }
};


// ==========================================
// 5. Singleton Management Pattern
// ==========================================

class NotificationService {
private:
    NotificationObservable* observable;
    static NotificationService* instance;
    vector<INotification*> notifications; // Tracks historical messages

    // Private Constructor
    NotificationService() {
        observable = new NotificationObservable();
    }

public:
    static NotificationService* getInstance() {
        if (instance == nullptr) {
            instance = new NotificationService();
        }
        return instance;
    }

    NotificationObservable* getObservable() {
        return observable;
    }

    void sendNotification(INotification* notification) {
        notifications.push_back(notification);
        observable->setNotification(notification);
    }

    ~NotificationService() {
        delete observable;
        for (auto n : notifications) {
            delete n;
        }
    }
};

// Initialize static member
NotificationService* NotificationService::instance = nullptr;


// ==========================================
// 6. Application Driver Block
// ==========================================
int main() {
    // Get Singleton Service
    NotificationService* notificationService = NotificationService::getInstance();

    // Get Observable Core
    NotificationObservable* notificationObservable = notificationService->getObservable();

    // Create Logger Observer
    Logger* logger = new Logger(notificationObservable);

    // Create Engine Observer and populate with routing Strategies
    NotificationEngine* notificationEngine = new NotificationEngine(notificationObservable);
    notificationEngine->addNotificationStrategy(new EmailStrategy("random.person@gmail.com"));
    notificationEngine->addNotificationStrategy(new SMSStrategy("+91 9876543210"));
    notificationEngine->addNotificationStrategy(new PopUpStrategy());

    // Attach systems to downstream events
    notificationObservable->addObserver(logger);
    notificationObservable->addObserver(notificationEngine);

    // Assemble dynamic formatting decorators
    INotification* notification = new SimpleNotification("Your order has been shipped!");
    notification = new TimestampDecorator(notification);
    notification = new SignatureDecorator(notification, "Customer Care");

    // Process notification broadcast across all channels
    notificationService->sendNotification(notification);

    // Cleanup local observer constructs (Singleton handles inner storage)
    delete logger;
    delete notificationEngine;
    delete notificationService; // Optional explicitly depending on application lifecycle hooks
    
    return 0;
}