//
// Created by ky3he4ik on 12/12/2019.
//

#ifndef ECC_MAINWINDOW_H
#define ECC_MAINWINDOW_H

#include <QWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QString>
#include <QVector>

#include "../curve/ECC.h"
#include <bitset>

class MainWindow : public QWidget {
Q_OBJECT

private:
    QGridLayout *layout;
    QVBoxLayout *buttons_layout;

    QLabel *label_a;
    QLabel *label_b;
    QLabel *label_p;
    QLabel *label_base_point;
    QLabel *label_order;
    QLabel *label_private;
    QLabel *label_public;
    QLabel *label_other_public;
    QLabel *label_shared;
    QLabel *label_clear_text;
    QLabel *label_cypher;
    QLabel *label_base64;
    QLabel *label_sign;

    QLineEdit *edit_a;
    QLineEdit *edit_b;
    QLineEdit *edit_p;
    QLineEdit *edit_base_point;
    QLineEdit *edit_order;
    QLineEdit *edit_private;
    QLineEdit *edit_public;
    QLineEdit *edit_other_public;
    QLineEdit *edit_shared;

    QTextEdit *edit_clear_text;
    QTextEdit *edit_cypher;
    QTextEdit *edit_base64;
    QTextEdit *edit_sign;

    QPushButton *button_load_curve;
    QPushButton *button_load_key_pair;
    QPushButton *button_load_other;
    QPushButton *button_save_curve;
    QPushButton *button_save_key_pair;
    QPushButton *button_encode;
    QPushButton *button_decode;

    QGroupBox *group_curve, *group_len;
    QVector<QPushButton *> curve_options;
    QVector<QPushButton *> len_options;

    ECC ecc;

    std::bitset<9> is_for_point = {0b000100110};

    void set_len(int new_len);

public:

    explicit MainWindow(QWidget * = Q_NULLPTR);

private slots:

    void buttonLoadCurveSlot();

    void buttonLoadKeyPairSlot();

    void buttonLoadOtherSlot();

    void buttonSaveCurveSlot();

    void buttonSaveKeyPairSlot();

    void buttonEncodeSlot();

    void buttonDecodeSlot();

    void editCypherChangedSlot();

    void editBase64ChangedSlot();

    void groupCurvePressed();

    void groupLenPressed();

    void editCurveChanged();

    void editPrivateChanged();

    void editOtherChanged();

};


#endif //ECC_MAINWINDOW_H
