# 🔔 Notification System (C++)

A simple C++ project that demonstrates how to design a notification system using object-oriented programming and common design patterns.

The example builds a notification message, adds formatting to it, and sends it through multiple channels such as Email, SMS, and Popup. A logger also observes new notifications.

## ✨ Features

- **Multiple delivery channels:** Email, SMS, and Popup strategies.
- **Dynamic message formatting:** Add a timestamp and signature without changing the original notification class.
- **Observer-based updates:** Notify the logging and delivery components when a new notification is created.
- **Central notification service:** A Singleton provides access to the notification service.
- **Object-oriented design:** Uses interfaces, inheritance, and polymorphism to separate responsibilities.

## 🧠 Design Patterns Used

| Pattern | Where it is used | Purpose |
| --- | --- | --- |
| **Decorator** | `TimestampDecorator`, `SignatureDecorator` | Adds extra content to a notification dynamically. |
| **Observer** | `NotificationObservable`, `Logger`, `NotificationEngine` | Notifies registered observers when a notification changes. |
| **Strategy** | `INotificationStrategy`, `EmailStrategy`, `SMSStrategy`, `PopUpStrategy` | Allows the delivery channel to vary independently. |
| **Singleton** | `NotificationService` | Provides one central service instance. |

## ⚙️ How It Works

1. The application creates a basic `SimpleNotification`.
2. `TimestampDecorator` adds a timestamp to the message.
3. `SignatureDecorator` adds a signature.
4. The `NotificationService` passes the message to `NotificationObservable`.
5. The observable notifies its registered observers.
6. `Logger` prints the notification, while `NotificationEngine` sends it through each configured strategy.

### Example Message

The program uses the message `Your order has been shipped!`. Its formatted content is similar to:

```text
[2026-10-21 12:49:32] Your order has been shipped!
-- Customer Care
```

The timestamp in the current example is hard-coded; it does not automatically use the current system time.

## 🗂️ Main Classes

- `INotification`: Interface for notification content.
- `SimpleNotification`: Stores the original message.
- `INotificationDecorator`: Base class for decorators.
- `TimestampDecorator` and `SignatureDecorator`: Add extra message content.
- `IObserver` and `IObservable`: Define the observer interfaces.
- `NotificationObservable`: Stores the current notification and notifies observers.
- `INotificationStrategy`: Interface for a delivery channel.
- `EmailStrategy`, `SMSStrategy`, and `PopUpStrategy`: Demonstration delivery strategies.
- `NotificationEngine`: Sends notification content through configured strategies.
- `Logger`: Prints notification content for logging.
- `NotificationService`: Central service used by the application.

## 🛠️ Requirements

- A C++ compiler such as **g++**
- C++11 or later

The project uses only standard C++ headers, so no third-party libraries are required.

## 🚀 Build and Run

Save the source code in a file named `main.cpp`, then run:

```bash
g++ -std=c++11 -Wall -Wextra -pedantic main.cpp -o notification_system
```

Run on Linux or macOS:

```bash
./notification_system
```

On Windows, if you use MinGW g++, run:

```powershell
.\notification_system.exe
```

## 📌 Important Notes

- Email, SMS, and Popup strategies currently print to the console. They do **not** connect to real email or SMS providers.
- The example is intended for learning design patterns and object-oriented design, not as a production notification service.
- **Memory ownership needs improvement before extending the project.** In the supplied implementation, both `NotificationService` and `NotificationObservable` delete notification pointers, which can cause double deletion. The Singleton is also manually deleted while its static pointer remains set. Consider using `std::unique_ptr` and a function-local static Singleton, or otherwise define one clear owner for each object.
- The observer list stores raw pointers. Observers should be detached or managed safely before they are destroyed.

## 🔮 Possible Improvements

- Use `std::unique_ptr` to make object ownership explicit.
- Generate timestamps from the system clock.
- Add real email/SMS provider integrations behind the strategy interface.
- Add unit tests for decorators, observer notifications, and delivery strategies.
- Add validation and error handling for delivery failures.

## 📚 Learning Goals

This project is useful for practicing:

- Interfaces and abstract classes
- Inheritance and runtime polymorphism
- Composition and object ownership
- The Decorator, Observer, Strategy, and Singleton patterns
- Separation of responsibilities in C++

---

Made as a C++ design-pattern learning project.
