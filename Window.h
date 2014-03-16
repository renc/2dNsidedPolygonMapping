
#include <QtWidgets>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *pP);
	void setRenderPointType(int iType) { m_iRenderPointType = iType; }
	void setPolyPos(const QVector<QVector3D> &aPolyPos) { m_aPolyPos = aPolyPos; repaint(); };
	void setLinePos(const QVector3D &p1, const QVector3D &p2) { m_p1 = p1; m_p2 = p2; update(); }

public slots:
	void animate();
	void onTChanged(int iT);
	
protected:
    void paintEvent(QPaintEvent *event);
private:
	int m_iElapsed;
	float m_fT;
	int m_iRenderPointType;
	QVector<QVector3D> m_aPolyPos;
	QVector3D m_p1, m_p2; 
};

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow();	   

public slots:		   
	void onPChanged(); 

private:
	Widget *pW1; Widget *pW2; Widget *pW3; 
	QLineEdit *pLE10; QLineEdit *pLE11; QLineEdit *pLE20; QLineEdit *pLE21; 
};
