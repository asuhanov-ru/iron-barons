#include <unordered_map>
#include <memory>
#include <functional>

#include <QApplication>
#include <QDir>
#include <QSplashScreen>
#include <QTimer>
#include <QtCore/QLoggingCategory>


int main(int argc, char* argv[]) {
	signal( SIGSEGV, handler );
}