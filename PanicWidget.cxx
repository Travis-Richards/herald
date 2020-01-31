#include "PanicWidget.h"

#include <QLabel>
#include <QHBoxLayout>

PanicWidget::PanicWidget(QWidget* parent) : QWidget(parent) {

  label = new QLabel(this);
  label->setAlignment(Qt::AlignCenter);

  layout = new QHBoxLayout(this);
  layout->addWidget(label, Qt::AlignCenter);

  setLayout(layout);
}

void PanicWidget::setText(const QString& message) {
  label->setText(message);
}
