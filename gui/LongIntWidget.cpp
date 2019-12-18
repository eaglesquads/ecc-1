//
// Created by ky3he4ik on 12/12/2019.
//

#include "LongIntWidget.h"

LongIntWidget::LongIntWidget(const QString &_label, bool _is_for_point, QWidget *) : is_for_point(_is_for_point) {
    layout = new QHBoxLayout();

    label = new QLabel(_label);
    layout->addWidget(label);

//    auto scrollArea = new QScrollArea();
    line = new QLineEdit();
//    scrollArea->setWidget(line);
//    scrollArea->setFixedHeight(line->height());
//    scrollArea->setWidgetResizable(true);

//    layout->addWidget(scrollArea);
    layout->addWidget(line);

    setLayout(layout);

//    line->showMaximized();
    connect(line, &QLineEdit::editingFinished, this, &LongIntWidget::contentChangedSlot);
}

QString LongIntWidget::getContents() const {
    return line->text();
}

void LongIntWidget::setContents(const QString &new_content) {
    line->setText(new_content);
}

//void LongIntWidget::setEnabled(bool enabled) {
//    line->setEnabled(enabled);
//}

void LongIntWidget::contentChangedSlot() {
    emit contentChangedSignal();
}

void LongIntWidget::setInputLen(int input_len) {
    QString input_mask;
    if (is_for_point)
        input_mask += "0H ";
    for (int i = 0; i < input_len + 15; i += 16)
        input_mask += "HHHH ";
    line->setInputMask(">" + input_mask + ";0");
//    if (line->text().isEmpty())
//        line->setText(input_mask.replace('H', '0'));
}