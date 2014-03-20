#ifndef ADDKTOGROUP__HPP__
#define ADDKTOGROUP__HPP__

#include <QWidget>

#include <vector>

class Kontact;
class Group;
class MainWindow;
class QHBoxLayout;
class QVBoxLayout;
class QCheckBox;
class QLabel;
class QPushButton;

class AddKToGroup: public QWidget
{
	Q_OBJECT
	public:
		AddKToGroup(Group*, MainWindow*);
	public slots:
		void submit();
	protected:
		Group* mGroup;
		MainWindow* mW;
		QVBoxLayout* mainLayout;
		QVBoxLayout* secLayout;
		QPushButton* mSubmit;
		QLabel* titre;
		std::vector<Kontact*> local;
		std::vector<Group*>* groups;
		std::vector<Group*> localg;
		std::vector<Group*> locali;
		std::vector<QCheckBox*> qcbs;
		std::vector<QCheckBox*> gqcbs;
		std::vector<QCheckBox*> iqcbs;
		QLabel* include;
		QLabel* rmInc;
		QVBoxLayout* thirdLayout;
		QVBoxLayout* fourthLayout;
};

#endif 
