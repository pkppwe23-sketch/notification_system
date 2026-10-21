# 🔔 Notification System (C++)

A beginner-friendly C++ project demonstrating four object-oriented design patterns in a notification system.

The program decorates a message with a timestamp and signature, then sends the formatted notification through Email, SMS, and Popup strategies. A logger observes the notification event.

## Features

- Multiple delivery strategies: Email, SMS, and Popup
- Dynamic message formatting with decorators
- Observer-based notification updates
- A central Singleton notification service
- Interfaces, inheritance, and runtime polymorphism

## Design Patterns Used

| Pattern | Classes | Purpose |
| --- | --- | --- |
| **Decorator** | `INotificationDecorator`, `TimestampDecorator`, `SignatureDecorator` | Adds formatting to a notification without modifying the base notification class. |
| **Observer** | `IObserver`, `IObservable`, `NotificationObservable`, `Logger`, `NotificationEngine` | Notifies registered observers when a notification is sent. |
| **Strategy** | `INotificationStrategy`, `EmailStrategy`, `SMSStrategy`, `PopUpStrategy` | Separates notification content from the delivery channel. |
| **Singleton** | `NotificationService` | Provides a central service instance. |

## How It Works

1. Create a `SimpleNotification` with the message `Your order has been shipped!`.
2. Wrap it in a `TimestampDecorator`.
3. Wrap that result in a `SignatureDecorator`.
4. Register `Logger` and `NotificationEngine` as observers.
5. Send the decorated notification through `NotificationService`.
6. The logger prints the message, and the engine sends it through all configured strategies.

The timestamp is hard-coded in this demonstration; it does not read the current system time.

## Main Classes

- **`INotification`** — interface for notification content.
- **`SimpleNotification`** — stores the original message.
- **`INotificationDecorator`** — base class for decorators and owns the wrapped notification.
- **`TimestampDecorator` / `SignatureDecorator`** — add a timestamp and signature.
- **`NotificationObservable`** — stores observer pointers and broadcasts updates.
- **`INotificationStrategy`** — interface for delivery channels.
- **`EmailStrategy` / `SMSStrategy` / `PopUpStrategy`** — console-based delivery strategies.
- **`NotificationEngine`** — sends the notification through each configured strategy.
- **`Logger`** — prints notification content for logging.
- **`NotificationService`** — Singleton entry point for sending notifications.

## Requirements

- A C++ compiler such as g++
- C++11 or later
- No third-party libraries

## Build and Run

Compile:

```bash
g++ -std=c++11 -Wall -Wextra -pedantic main.cpp -o notification_system
```

Run on Linux/macOS:

```bash
./notification_system
```

Run on Windows with MinGW:

```powershell
.\notification_system.exe
```

## Important Memory-Ownership Warning

This version follows the raw-pointer implementation provided for the project, but **it currently has a double-delete bug**:

- `NotificationObservable` deletes `currentNotification` in its destructor.
- `NotificationService` also deletes the same notification through its `notifications` vector.
- In addition, `setNotification()` can delete a previous notification that is still stored in `notifications`.

These overlapping ownership responsibilities can cause undefined behavior. The Singleton is also manually deleted while its static `instance` pointer is left non-null. Before using or extending this as a running application, fix ownership so each object has exactly one clear owner (for example, by redesigning around `std::unique_ptr` and a function-local static Singleton). The observer list also contains non-owning raw pointers, so observers must be detached before destruction.

Email, SMS, and Popup currently print to the console; they do not connect to real delivery providers.

## Possible Improvements

- Make ownership safe and explicit with smart pointers.
- Generate timestamps using the system clock.
- Integrate actual email/SMS providers.
- Add unit tests and delivery-failure handling.

---

Made as a C++ design-pattern learning project.
