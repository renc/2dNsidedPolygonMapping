
#include "Window.h"

QVector<double> CoordFromPos(const QVector<QVector3D> &m_aPolyPos, const QVector3D &p)
{
	QVector<float> aTan(m_aPolyPos.size());
	for (int iP = 0; iP < m_aPolyPos.size(); ++iP)
	{
		QVector3D v0 = (m_aPolyPos[iP] - p).normalized();
		QVector3D v1 = (m_aPolyPos[(iP+1)%m_aPolyPos.size()] - p).normalized();

		double fAngle =  acos(QVector3D::dotProduct(v0, v1)) / 2;
		aTan[iP] = tan(fAngle);
	}
	QVector<double> aCoord(m_aPolyPos.size());
	double fSum = 0;
	for (int iP = 0; iP < m_aPolyPos.size(); ++iP)
	{
		double fL = (m_aPolyPos[iP] - p).length();
		if (fL < 1e-6)
			aCoord[iP] = 1;
		else
			aCoord[iP] = (aTan[ (iP + m_aPolyPos.size() - 1) % m_aPolyPos.size() ] + aTan[iP]) / fL; 
		fSum += aCoord[iP];
	}
	double fSumTest = 0;
	for (int iP = 0; iP < aCoord.size(); ++iP)
	{
		aCoord[iP] = aCoord[iP] / fSum;
		fSumTest += aCoord[iP];
	}
	//qDebug() << fSumTest; 
	return aCoord;
} 

QVector3D CoordToPos(const QVector<QVector3D> &m_aPolyPos, const QVector<double> &aCoord)
{
	QVector3D p(0, 0, 0);
	for (int iP = 0; iP < m_aPolyPos.size(); ++iP)
	{
		p += m_aPolyPos[iP] * aCoord[iP];
	}
	return p;
}

Widget::Widget(QWidget *pP) : QWidget(pP) 
{
	m_iElapsed = 0;
	m_fT = 0; 
	m_iRenderPointType = 0;
	m_aPolyPos.clear();

	//setFixedSize(200, 200); // this will fix the widget/paint region.
	setMinimumSize(300, 300);
}

void Widget::animate()
{			 
    m_iElapsed = (m_iElapsed + qobject_cast<QTimer*>(sender())->interval()) % 1000;
    repaint();
} 

void Widget::onTChanged(int iT)
{
	m_fT = iT / 100.0; 
	repaint();
} 

void Widget::paintEvent(QPaintEvent *event)
{
	//
	QVector3D pMid = m_p1 + (m_p2 - m_p1) * m_fT;
	
	QVector<double> aC1 = CoordFromPos(m_aPolyPos, m_p1); 
	QVector<double> aC2 = CoordFromPos(m_aPolyPos, m_p2);
	QVector<double> aCMid = CoordFromPos(m_aPolyPos, pMid);
	QVector<double> aCMidCalc(aC1.size());
	for (int i = 0; i < aC1.size(); ++i)
		aCMidCalc[i] = aC1[i] + (aC2[i] - aC1[i]) * m_fT; 												 
	QVector3D pMidFromCoord = CoordToPos(m_aPolyPos, aCMid); 
	QVector3D pMidCalc = CoordToPos(m_aPolyPos, aCMidCalc); 

	// 
	QBrush background;
														
	background = QBrush(QColor(64, 32, 64));
	
	int elapsed = m_iElapsed;	
	
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
	painter.fillRect(event->rect(), background);
    					  
	QPen polyLinePen(Qt::blue);
	polyLinePen.setWidth(1);
	painter.setPen(polyLinePen);
	QPolygonF polygon;
	for (int i = 0; i < m_aPolyPos.size(); ++i)
		polygon << m_aPolyPos[i].toPointF();
	painter.drawPolygon(polygon); 	
	painter.drawLine(m_p1.toPointF(), m_p2.toPointF());   
		
	QPen pointPen(Qt::green);
	pointPen.setWidth(4);
	painter.setPen(pointPen);

	painter.drawPoint(m_p1.toPointF());
	painter.drawPoint(m_p2.toPointF());

	{
		painter.setPen(pointPen);
		
		painter.drawPoint(pMid.toPointF());	 

		//
		QPen pointYellowPen(Qt::yellow);
		pointYellowPen.setWidth(4);
		painter.setPen(pointYellowPen);
		
		painter.drawPoint(pMidFromCoord.toPointF());
		
		//
		QPen pointRedPen(Qt::red);
		pointRedPen.setWidth(4);
		painter.setPen(pointRedPen);
		
		painter.drawPoint(pMidCalc.toPointF());
	}
	
	qDebug() << "--";
	qDebug() << "pos coord:";
	qDebug() << aC1;
	qDebug() << aC2;
	qDebug() << aCMid;	
	qDebug() << aCMidCalc;												
	qDebug() << "linear pos:";
	qDebug() << pMid;
	qDebug() << pMidFromCoord;
	qDebug() << pMidCalc;
	// 
	QFont textFont;
    QPen textPen;			   
	textPen = QPen(Qt::white);
	textFont.setPixelSize(50);

    painter.setPen(textPen);
    painter.setFont(textFont);
	painter.drawText(QRect(0, 200, event->rect().width(), event->rect().height() - 200),
		Qt::AlignLeft | Qt::AlignTop, 
		""
		);

	painter.end();
}

MainWindow::MainWindow()
{	   
	setWindowTitle(tr("Mapping demo"));
	
	QVector<QVector3D> aPolyPos3;
	aPolyPos3 
		<< QVector3D(100, 50, 0) 
		<< QVector3D(50, 150, 0)
		<< QVector3D(250, 200, 0);
	
	QVector<QVector3D> aPolyPos4;
	aPolyPos4 
		<< QVector3D(100, 50, 0) << QVector3D(50, 100, 0) 
		<< QVector3D(50, 150, 0)
		<< QVector3D(250, 200, 0);
			 
	QVector<QVector3D> aPolyPos5;
	aPolyPos5 
		<< QVector3D(100, 50, 0) << QVector3D(50, 100, 0) 
		<< QVector3D(50, 150, 0)
		<< QVector3D(250, 200, 0) 
		<< QVector3D(200, 50, 0); 
	
	QVector3D p1(100, 100, 0);
	QVector3D p2(200, 150, 0);

	// ui. 
	QSlider *pSlider = new QSlider(this); pSlider->setRange(-100, 400);//pSlider->setRange(0, 100);
	
	pW1 = new Widget(this); pW1->setPolyPos(aPolyPos3); pW1->setLinePos(p1, p2);
	pW2 = new Widget(this); pW2->setPolyPos(aPolyPos4);	pW2->setLinePos(p1, p2);
	pW3 = new Widget(this); pW3->setPolyPos(aPolyPos5);	pW3->setLinePos(p1, p2);
									 
	connect(pSlider, SIGNAL(valueChanged(int)), pW1, SLOT(onTChanged(int)));
	connect(pSlider, SIGNAL(valueChanged(int)), pW2, SLOT(onTChanged(int)));
	connect(pSlider, SIGNAL(valueChanged(int)), pW3, SLOT(onTChanged(int)));
	
	QVBoxLayout *pVBLayout = new QVBoxLayout;

	QHBoxLayout *pHBLayout = new QHBoxLayout;
	pHBLayout->addWidget(pSlider); pHBLayout->addWidget(pW1); pHBLayout->addWidget(pW2); pHBLayout->addWidget(pW3);
	pVBLayout->addLayout(pHBLayout);
	
	pLE10 = new QLineEdit; pLE10->setValidator(new QIntValidator()); pLE10->setText(QString::number(p1.x())); connect(pLE10, SIGNAL(editingFinished()), this, SLOT(onPChanged()));
	pLE11 = new QLineEdit; pLE11->setValidator(new QIntValidator()); pLE11->setText(QString::number(p1.y())); connect(pLE11, SIGNAL(editingFinished()), this, SLOT(onPChanged()));
	pLE20 = new QLineEdit; pLE20->setValidator(new QIntValidator()); pLE20->setText(QString::number(p2.x())); connect(pLE20, SIGNAL(editingFinished()), this, SLOT(onPChanged()));
	pLE21 = new QLineEdit; pLE21->setValidator(new QIntValidator()); pLE21->setText(QString::number(p2.y())); connect(pLE21, SIGNAL(editingFinished()), this, SLOT(onPChanged()));
	pHBLayout = new QHBoxLayout; 
	pHBLayout->addWidget(new QLabel("p1 pos:")); pHBLayout->addWidget(pLE10); pHBLayout->addWidget(pLE11);	
	pHBLayout->addWidget(new QLabel("p2 pos:")); pHBLayout->addWidget(pLE20); pHBLayout->addWidget(pLE21);
	pVBLayout->addLayout(pHBLayout); 

	QWidget *pCenterWidget = new QWidget;
	pCenterWidget->setLayout(pVBLayout);
	setCentralWidget(pCenterWidget); // inevitably.
																		  
}

void MainWindow::onPChanged()
{	
	if (!pLE10 || !pLE11 || !pLE20 || !pLE21)
		return;

	float fP10 = pLE10->text().toFloat(); float fP11 = pLE11->text().toFloat(); 
	float fP20 = pLE20->text().toFloat(); float fP21 = pLE21->text().toFloat(); 
	pW1->setLinePos(QVector3D(fP10, fP11, 0), QVector3D(fP20, fP21, 0)); 
	pW2->setLinePos(QVector3D(fP10, fP11, 0), QVector3D(fP20, fP21, 0)); 
	pW3->setLinePos(QVector3D(fP10, fP11, 0), QVector3D(fP20, fP21, 0)); 
}

