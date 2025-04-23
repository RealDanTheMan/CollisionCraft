#include "propertypanel.h"
#include "collisiongen.h"
#include "logging.h"

#include <QVBoxLayout>
#include <QLabel>

PropertyPanelWidget::PropertyPanelWidget(QWidget *parent) : 
    QWidget(parent),
    selected_technique(CollisionTechnique::SimpleHull)
{
    QLabel *technique_label = new QLabel("Collision Generation Technique", this);

    this->technique_menu = new QComboBox(this);
    this->technique_menu->addItem(
        "Volume Hull",
        QVariant::fromValue(static_cast<int>(CollisionTechnique::SimpleHull))
    );
    this->technique_menu->addItem(
        "Exact Decomposition",
        QVariant::fromValue(static_cast<int>(CollisionTechnique::ExactDecomposition))
    );

    this->generate_button = new QPushButton("Generate Collision", this);
    this->generate_button->setMinimumHeight(32);

    connect(
        this->generate_button,
        &QPushButton::clicked,
        this,
        &PropertyPanelWidget::collisionGenerationRequested
    );

    connect(
        this->technique_menu,
        &QComboBox::currentIndexChanged,
        this,
        &PropertyPanelWidget::onTechniqueSelectionChanged
    );

    QVBoxLayout *panel_layout = new QVBoxLayout();
    panel_layout->addStretch();
    panel_layout->setContentsMargins(QMargins(0.0, 0.0, 0.0, 0.0));
    panel_layout->setSpacing(0);
    panel_layout->addWidget(technique_label);
    panel_layout->addWidget(this->technique_menu);
    panel_layout->addWidget(this->generate_button);

    this->setLayout(panel_layout);
}

/// Get currently selected value in collision generation drop-down menu.
CollisionTechnique PropertyPanelWidget::getSelectedTechnique() const
{
    return this->selected_technique;
}

/// Event handler invoked when user changes the 'Collision Generation Technique' menu option.
void PropertyPanelWidget::onTechniqueSelectionChanged()
{
    const int idx = this->technique_menu->currentIndex();
    const int technique_id = this->technique_menu->itemData(idx).toInt();

    this->selected_technique = static_cast<CollisionTechnique>(technique_id);
    logDebug("Selected collision technique changed -> {}", technique_id);
}
