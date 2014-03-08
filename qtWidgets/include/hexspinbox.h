#ifndef HEXSPINBOX_H
#define HEXSPINBOX_H

#include <QSpinBox>
#include <QtGui/QDockWidget>

class HexSpinBox : public QSpinBox
{
    Q_OBJECT

public:
          HexSpinBox(QWidget *parent = 0);
         // HexSpinBox(QDockWidget *parent = 0);

protected:
          QValidator::State validate(QString &text, int &pos) const;
          int valueFromText (const QString &text) const;
          QString textFromValue(int value) const;
private:
          QRegExpValidator *validator;
};

class QRegExpValidator;

#endif // HEXSPINBOX_H
