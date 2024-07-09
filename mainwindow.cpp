#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "mesure.hpp"
#include <stdio.h>
#include <string>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  this->current_vertical = 0;
  vertical verticale_vide = vertical();
  this->current_mesure.ajout_mesure(verticale_vide);
  this->setupPlot(ui->customPlot);
  this->setupFond(ui->customPlot);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_ouvrirButton_clicked() {
    QString filename = QFileDialog::getOpenFileName(this, "Open XML File", "", "XML Files (*.xml)");
    if (!filename.isEmpty()) {
        current_mesure.from_xml(filename.toStdString());
        update_display();
        QMessageBox::information(this, "Success", "XML file loaded successfully.");
    }
}

void MainWindow::on_enregistrerButton_clicked() {
    QString filename = QFileDialog::getSaveFileName(this, "Save XML File", "", "XML Files (*.xml)");
    if (!filename.isEmpty()) {
        current_mesure.to_xml(this->current_mesure.debitmetre, filename.toStdString());
        QMessageBox::information(this, "Success", "XML file saved successfully.");
    }
}

void MainWindow::on_add_vert_clicked() {
  vertical verticale_vide = vertical();
  this->current_mesure.ajout_mesure(verticale_vide);
  this->current_vertical =
  this->current_mesure.ensemble_des_mesures_des_verticales.size() - 1;
  this->update_display();
}

void MainWindow::on_prec_vert_clicked() {
  if (this->current_vertical > 0) {
    this->current_vertical--;
  }
  this->update_display();
}

void MainWindow::on_next_vert_clicked() {
  if (this->current_vertical <
      this->current_mesure.ensemble_des_mesures_des_verticales.size() - 1) {
    this->current_vertical++;
  }
  this->update_display();
}

void MainWindow::on_add_mes_clicked() {
  une_mesure mesure_vide;
  this->current_mesure
      .ensemble_des_mesures_des_verticales[this->current_vertical]
      .ajout_mesure(mesure_vide);
  ui->tableau_mesures->insertRow(ui->tableau_mesures->rowCount());
  this->update_display();
}

void MainWindow::update_display() {
  this->setupFond(ui->customPlot);
  std::string s = "Verticale ";
  s += std::to_string(this->current_vertical + 1);
  s += "/";
  s += std::to_string(
      this->current_mesure.ensemble_des_mesures_des_verticales.size());
  ui->label_vert_count->setText(s.c_str());

  int n_rows = this->current_mesure
                   .ensemble_des_mesures_des_verticales[this->current_vertical]
                   .les_mesures_de_la_vertical.size();

  ui->tableau_mesures->setRowCount(n_rows);

  for (int i = 0; i < n_rows; i++) {
    QTableWidgetItem *item = new QTableWidgetItem();
    item->setText(
        std::to_string(
            this->current_mesure
                .ensemble_des_mesures_des_verticales[this->current_vertical]
                .les_mesures_de_la_vertical[i]
                .profondeur_mesure)
            .c_str());
    ui->tableau_mesures->setItem(i, 0, item);
    item = new QTableWidgetItem();
    item->setText(
        std::to_string(
            this->current_mesure
                .ensemble_des_mesures_des_verticales[this->current_vertical]
                .les_mesures_de_la_vertical[i]
                .nombre_tops)
            .c_str());
    ui->tableau_mesures->setItem(i, 1, item);
  }

  ui->input_dist->setText(
      std::to_string(
          this->current_mesure
              .ensemble_des_mesures_des_verticales[this->current_vertical]
              .distance)
          .c_str());
  ui->input_prof->setText(
      std::to_string(
          this->current_mesure
              .ensemble_des_mesures_des_verticales[this->current_vertical]
              .profondeur)
          .c_str());

  std::string s2 = "Débit total : ";
  double x = this->current_mesure.debit();
  fprintf(stderr, "debit : %d\n", x);
  std::cout << x << std::endl;
  s2 += std::to_string(x);
  ui->debit->setText(s2.c_str());
}

void MainWindow::on_tableau_mesures_cellChanged(int row, int column) {
  QString item = ui->tableau_mesures->item(row, column)->text();
  QByteArray ba = item.toLocal8Bit();
  const char *c_str = ba.data();
  try {
    if (column == 0) {
      this->current_mesure
          .ensemble_des_mesures_des_verticales[this->current_vertical]
          .les_mesures_de_la_vertical[row]
          .profondeur_mesure = stod(c_str);
    } else if (column == 1) {
      this->current_mesure
          .ensemble_des_mesures_des_verticales[this->current_vertical]
          .les_mesures_de_la_vertical[row]
          .nombre_tops = stol(c_str);
    }
  } catch (...) {
    ui->tableau_mesures->item(row, column)->setText("ERR");
  }
}

void MainWindow::on_input_dist_editingFinished() {
  QString inpt = ui->input_dist->text();
  QByteArray ba = inpt.toLocal8Bit();
  const char *c_str = ba.data();
  try {
    this->current_mesure
        .ensemble_des_mesures_des_verticales[this->current_vertical]
        .distance = stol(c_str);
  } catch (...) {
    ui->input_dist->setText("ERR");
  }
  this->update_display();
}

void MainWindow::on_input_prof_editingFinished() {
  QString inpt = ui->input_prof->text();
  QByteArray ba = inpt.toLocal8Bit();
  const char *c_str = ba.data();
  try {
    this->current_mesure
        .ensemble_des_mesures_des_verticales[this->current_vertical]
        .profondeur = stod(c_str);
  } catch (...) {
    ui->input_prof->setText("ERR");
  }
  this->update_display();
}

void MainWindow::on_input_coeff_fond_editingFinished() {
  QString inpt = ui->input_coeff_fond->text();
  QByteArray ba = inpt.toLocal8Bit();
  const char *c_str = ba.data();
  try {
    this->current_mesure.coeff_de_fond = stod(c_str);
  } catch (...) {
    ui->input_coeff_fond->setText("ERR");
  }
}

void MainWindow::on_input_coeff_bord_editingFinished() {
  QString inpt = ui->input_coeff_bord->text();
  QByteArray ba = inpt.toLocal8Bit();
  const char *c_str = ba.data();
  try {
    this->current_mesure.coeff_de_bord = stod(c_str);
  } catch (...) {
    ui->input_coeff_fond->setText("ERR");
  }
  this->update_display();
}

void MainWindow::on_input_dbgauche_editingFinished() {
  QString inpt = ui->input_dbgauche->text();
  QByteArray ba = inpt.toLocal8Bit();
  const char *c_str = ba.data();
  try {
    this->current_mesure.distance_bord_gauche = stod(c_str);
  } catch (...) {
    ui->input_dbgauche->setText("ERR");
  }
  this->update_display();
}

void MainWindow::on_input_dbdroit_editingFinished() {
  QString inpt = ui->input_dbdroit->text();
  QByteArray ba = inpt.toLocal8Bit();
  const char *c_str = ba.data();
  try {
    this->current_mesure.distance_bord_droit = stod(c_str);
  } catch (...) {
    ui->input_dbdroit->setText("ERR");
  }
}

void MainWindow::on_input_pbgauche_editingFinished() {
  QString inpt = ui->input_pbgauche->text();
  QByteArray ba = inpt.toLocal8Bit();
  const char *c_str = ba.data();
  try {
    this->current_mesure.hauteur_bord_gauche = stod(c_str);
  } catch (...) {
    ui->input_pbgauche->setText("ERR");
  }
  this->update_display();
}

void MainWindow::on_input_pbdroit_editingFinished() {
  QString inpt = ui->input_pbdroit->text();
  QByteArray ba = inpt.toLocal8Bit();
  const char *c_str = ba.data();
  try {
    this->current_mesure.hauteur_bord_droit = stod(c_str);
  } catch (...) {
    ui->input_pbdroit->setText("ERR");
  }
  this->update_display();
}

// void MainWindow::setupPlot(QCustomPlot *customPlot) {
//   // generate some data:
//   QVector<double> x(101), y(101); // initialize with entries 0..100
//   for (int i = 0; i < 101; ++i) {
//     x[i] = i / 50.0 - 1; // x goes from -1 to 1
//     y[i] = x[i] * x[i];  // let's plot a quadratic function
//   }
//   // create graph and assign data to it:
//   customPlot->addGraph();
//   customPlot->graph(0)->setData(x, y);
//
//   // set scatter style
//   customPlot->graph(0)->setLineStyle(QCPGraph::lsNone); // No line
//   customPlot->graph(0)->setScatterStyle(QCPScatterStyle(
//       QCPScatterStyle::ssCircle, 5)); // Circle scatter style with size 5
//
//   // give the axes some labels:
//   customPlot->xAxis->setLabel("x");
//   customPlot->yAxis->setLabel("y");
//   // set axes ranges, so we see all data:
//   customPlot->xAxis->setRange(-1, 1);
//   customPlot->yAxis->setRange(0, 1);
// }

void MainWindow::setupPlot(QCustomPlot *customPlot) {
    // when starting without any data in the plot, it is often useful to set the
    // range of the plot manually. This way, the user can't scroll/zoom out to a
    // range where the plot is empty.
    customPlot->xAxis->setRange(0, 10);
    customPlot->yAxis->setRange(0, 10);
    customPlot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void MainWindow::setupFond(QCustomPlot *customPlot) {
    // Retrieve the number of vertical measurements
    int numMesures = this->current_mesure.ensemble_des_mesures_des_verticales.size();

    // Check if there are measurements
    if (numMesures == 0) {
        qDebug() << "No vertical measurements available.";
        return;
    }

    // Initialize vectors to store distances and depths
    QVector<double> x(numMesures);
    QVector<double> y(numMesures);

    // Populate vectors with distances and depths from the measurements
    for (int i = 0; i < numMesures; ++i) {
        x[i] = this->current_mesure.ensemble_des_mesures_des_verticales[i].distance;
        y[i] = -this->current_mesure.ensemble_des_mesures_des_verticales[i].profondeur;
    }

    // Add a new graph to the custom plot
    customPlot->addGraph();
    customPlot->graph(0)->setData(x, y);

    // Set line style to a solid line (default style)
    customPlot->graph(0)->setLineStyle(QCPGraph::lsLine); // Line graph

    // Optionally, you can also set scatter style if you want markers on the line
    // customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));

    // Set labels for the axes
    customPlot->xAxis->setLabel("Distance");
    customPlot->yAxis->setLabel("Profondeur");

    // Rescale axes to fit the data
    customPlot->rescaleAxes();

    // Replot to update the view
    customPlot->replot();

    qDebug() << "setupFond has been called and graph has been updated.";
}

