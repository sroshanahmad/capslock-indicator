#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <X11/XKBlib.h>
#include <QVBoxLayout>
#include <QScreen>  // Include QScreen

class CapsLockIndicator : public QWidget {
public:
    CapsLockIndicator() {
        this->setWindowTitle("Caps Lock Status");
        this->setFixedSize(300, 100); // Fixed size for the window

        // Frameless window and semi-transparent background
        this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
        this->setAttribute(Qt::WA_TranslucentBackground);

        label = new QLabel("Caps Lock is ON", this);
        label->setAlignment(Qt::AlignCenter);

        // CSS-like styling for a modern look
        label->setStyleSheet(
            "QLabel {"
            "  color: #FFFFFF;"  // White text
            "  font: bold 18pt 'Arial';"  // Bold, larger font
            "  background-color: rgba(50, 50, 50, 180);"  // Semi-transparent dark background
            "  padding: 15px;"
            "  border-radius: 15px;"
            "  border: 2px solid #3A9AD9;"  // Light blue border
            "}"
        );

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(label);
        this->setLayout(layout);

        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &CapsLockIndicator::updateCapsLockStatus);
        timer->start(500);

        updateCapsLockStatus(); // Initial check

        // Move the window to the bottom-middle
        QScreen *screen = QGuiApplication::primaryScreen();
        QRect screenRect = screen->geometry();
        int x = (screenRect.width() - this->width()) / 2;  // Center horizontally
        int y = screenRect.height() - this->height();  // Bottom of the screen
        this->move(x, y);
    }

private:
    QLabel *label;

    void updateCapsLockStatus() {
        Display *d = XOpenDisplay((char*)0);
        bool caps_state = false;
        if (d) {
            unsigned int n;
            if (XkbGetIndicatorState(d, XkbUseCoreKbd, &n) == Success) {
                caps_state = (n & 0x01) == 1;
            }
            XCloseDisplay(d); // Close the X display when done
        }

        if (caps_state) {
            this->show();
        } else {
            this->hide();
        }
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    CapsLockIndicator window;
    return app.exec();
}
