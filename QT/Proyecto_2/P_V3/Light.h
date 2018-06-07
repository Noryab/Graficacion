#ifndef _Light_H
#define _Light_H

#include <QWidget>
#include <QObject>
#include <QtGui>
#include <QtCore>
#include <iostream>
#include <memory>

#include "Color.h"

class Source {
    public:

    Source();

    virtual QVector3D getLightPosition() {return QVector3D(0, 0, 0);}
    virtual QColor getLightColor() {return QColor(1, 1, 1, 0);}

};

Source::Source() {}


class Light : public Source {
    QVector3D position;
    QColor color;
	
	public:
	
	Light ();
	
    Light (QVector3D, QColor);
	
	// method functions
    virtual QVector3D getLightPosition () { return position; }
    virtual QColor getLightColor () { return color; }
	
};

Light::Light () {
    position = QVector3D(0,0,0);
    color = QColor(1,1,1, 0);
}

Light::Light (QVector3D p, QColor c) {
	position = p;
	color = c;
}




#endif
