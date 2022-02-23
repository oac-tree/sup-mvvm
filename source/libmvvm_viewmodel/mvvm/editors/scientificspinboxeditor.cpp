// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#include "mvvm/editors/scientificspinboxeditor.h"

#include "mvvm/editors/scientificspinbox.h"
#include "mvvm/utils/numericutils.h"

#include <QVBoxLayout>
#include <stdexcept>

namespace mvvm
{

ScientificSpinBoxEditor::ScientificSpinBoxEditor(QWidget* parent)
    : CustomEditor(parent), m_doubleEditor(new ScientificSpinBox)
{
  setAutoFillBackground(true);
  setFocusPolicy(Qt::StrongFocus);
  m_doubleEditor->setFocusPolicy(Qt::StrongFocus);
  m_doubleEditor->setKeyboardTracking(false);

  auto layout = new QVBoxLayout;
  layout->setMargin(0);
  layout->setSpacing(0);

  layout->addWidget(m_doubleEditor);

  connect(m_doubleEditor, &ScientificSpinBox::valueChanged, [=] { this->onEditingFinished(); });

  setLayout(layout);

  setFocusProxy(m_doubleEditor);
}

void ScientificSpinBoxEditor::setRange(double minimum, double maximum)
{
  m_doubleEditor->setMinimum(minimum);
  m_doubleEditor->setMaximum(maximum);
}

void ScientificSpinBoxEditor::setDecimals(int decimals)
{
  m_doubleEditor->setDecimals(decimals);
}

void ScientificSpinBoxEditor::setSingleStep(double step)
{
  m_doubleEditor->setSingleStep(step);
}

bool ScientificSpinBoxEditor::IsPersistent() const
{
  return true;
}

void ScientificSpinBoxEditor::onEditingFinished()
{
  double new_value = m_doubleEditor->value();

  if (!utils::AreAlmostEqual(new_value, GetData().value<double>()))
    SetDataIntern(QVariant::fromValue(new_value));
}

void ScientificSpinBoxEditor::UpdateComponents()
{
  if (GetData().type() != QVariant::Double)
    throw std::runtime_error(
        "ScientificSpinBoxEditor::update_components() -> Error. Wrong variant type");

  m_doubleEditor->setValue(GetData().value<double>());
}

}  // namespace mvvm
