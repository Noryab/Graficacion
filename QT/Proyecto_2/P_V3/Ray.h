#ifndef _RAY_H
#define _RAY_H

#include <QWidget>
#include <QObject>
#include <QtGui>
#include <QtCore>
#include <iostream>
#include <memory>


class Ray {
    QVector3D origin, direction;
	
	public:	
	Ray ();	
    Ray (QVector3D, QVector3D);
	
	// method functions
    QVector3D getRayOrigin () { return origin; }
    QVector3D getRayDirection () { return direction; }
	
};

Ray::Ray () {
    origin = QVector3D(0,0,0);
    direction = QVector3D(1,0,0);
}

Ray::Ray (QVector3D o, QVector3D d) {
	origin = o;
	direction = d;
}

#endif
