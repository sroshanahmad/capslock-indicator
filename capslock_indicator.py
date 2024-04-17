import sys
from PyQt5 import QtCore, QtWidgets
from Xlib import display

class CapsLockIndicator(QtWidgets.QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):
        self.setWindowFlags(QtCore.Qt.FramelessWindowHint | QtCore.Qt.WindowStaysOnTopHint)
        self.setAttribute(QtCore.Qt.WA_TranslucentBackground)

        # Enhanced modern styling for the label
        self.label = QtWidgets.QLabel("Caps Lock is ON", self)
        self.label.setStyleSheet("""
            QLabel {
                color: #FFFFFF;  /* White text */
                font: bold 18pt "Arial";  /* Bold, larger font */
                background-color: rgba(50, 50, 50, 180);  /* Semi-transparent dark background */
                padding: 15px;
                border-radius: 15px;
                border: 2px solid #3A9AD9; /* Light blue border */
            }
        """)
        self.label.adjustSize()  # Adjust size based on content and padding

        self.adjustSize()  # Adjust the window size to fit the label
        self.positionWindow()

    def positionWindow(self):
        screen_geometry = QtWidgets.QApplication.desktop().screenGeometry()
        x = (screen_geometry.width() - self.width()) // 2
        y = screen_geometry.height() - self.height()
        self.move(x, y)

    def update_caps_status(self):
        if self.is_caps_lock_on():
            self.show()
        else:
            self.hide()
        QtCore.QTimer.singleShot(500, self.update_caps_status)

    @staticmethod
    def is_caps_lock_on():
        d = display.Display()
        return d.get_keyboard_control().led_mask & 1

def main():
    app = QtWidgets.QApplication(sys.argv)
    ex = CapsLockIndicator()
    ex.update_caps_status()
    sys.exit(app.exec_())

if __name__ == '__main__':
    main()

