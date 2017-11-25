#include "Layout.h"

#include "bwCheckbox.h"
#include "bwLabel.h"
#include "bwNumberSlider.h"
#include "bwPushButton.h"
#include "bwStyleManager.h"

#include "DefaultStage.h"

using namespace bWidgetsDemo;
using namespace bWidgets; // Less verbose


#define BUTTON_HEIGHT 20

namespace bWidgetsDemo {

class ScaleSetter : public bwFunctorInterface
{
public:
	ScaleSetter(const bwNumberSlider&);
	void operator()() override;

private:
	const bwNumberSlider& numslider;
};

class StyleSetter : public bwFunctorInterface
{
public:
	StyleSetter(Stage& stage, const bwStyle::StyleType& style_type);
	static bool StyleButtonsUpdateCb(bwWidget&, void*);
	void operator()() override;

private:
	Stage& stage;
	const bwStyle::StyleType& style_type;
};

} // namespace bWidgetsDemo


// --------------------------------------------------------------------

DefaultStage::DefaultStage(unsigned int mask_width, unsigned int mask_height) :
    Stage(mask_width, mask_height)
{
	addStyleSelector(*layout);

	bwNumberSlider* slider = new bwNumberSlider(0, BUTTON_HEIGHT);
	slider->apply_functor = std::unique_ptr<ScaleSetter>(new ScaleSetter(*slider));
	slider->setText("Interface Scale: ");
	slider->setMinMax(0.5f, 2.0f);
	slider->setValue(1.0f);
	layout->addWidget(slider);

	addFakeSpacer(*layout);

	ColumnLayout* col = new ColumnLayout(true, layout);
	col->addWidget(new bwPushButton("Translate", 0, BUTTON_HEIGHT));
	col->addWidget(new bwPushButton("Rotate", 0, BUTTON_HEIGHT));
	col->addWidget(new bwPushButton("Scale", 0, BUTTON_HEIGHT));

	layout->addWidget(new bwPushButton("Mirror", 0, BUTTON_HEIGHT));

	addFakeSpacer(*layout);

	RowLayout* row = new RowLayout(true, layout);
	row->addWidget(new bwCheckbox("Make Awesome", 0, BUTTON_HEIGHT));
	row->addWidget(new bwCheckbox("Wireframes", 0, BUTTON_HEIGHT));

	bwTextBox* text_box = new bwTextBox(0, BUTTON_HEIGHT);
	text_box->setText("Some Text...");
	layout->addWidget(text_box);
}

void DefaultStage::addStyleSelector(LayoutItem& parent_layout)
{
	RowLayout* row_layout = new RowLayout(true, &parent_layout);

	row_layout->addWidget(new bwLabel("Style:", 0, BUTTON_HEIGHT));

	for (const bwStyle::StyleType& type : bwStyleManager::getStyleManager().getBuiltinStyleTypes()) {
		bwRadioButton* style_button = new bwRadioButton(type.name, 0, BUTTON_HEIGHT);

		style_button->apply_functor = std::unique_ptr<StyleSetter>(new StyleSetter(*this, type));

		if (type.type_id == style->type_id) {
			style_button->state = bwAbstractButton::STATE_SUNKEN;
		}

		row_layout->addWidget(style_button);
	}
}

void DefaultStage::addFakeSpacer(LayoutItem& layout)
{
	// Just some extra space. No spacer widgets yet.
	layout.addWidget(new bwLabel("", 0, 7));
}

// --------------------------------------------------------------------
// Functor definitions

ScaleSetter::ScaleSetter(const bwNumberSlider& numslider) :
    numslider(numslider)
{
	
}

void ScaleSetter::operator()()
{
	Stage::setInterfaceScale(numslider.getValue());
}

StyleSetter::StyleSetter(Stage& stage, const bwStyle::StyleType& style_type) :
    stage(stage), style_type(style_type)
{
	
}

bool StyleSetter::StyleButtonsUpdateCb(bwWidget& widget_iter, void* _active_style_id)
{
	auto active_style_id = *static_cast<bwStyle::StyleTypeID*>(_active_style_id);
	auto radio_iter = widget_cast<bwRadioButton*>(&widget_iter);

	if (radio_iter && radio_iter->apply_functor) {
		// Using dynamic_cast to check if apply_functor is a StyleSetter. Then we assume it's a style button.
		if (auto iter_style_setter = dynamic_cast<StyleSetter*>(radio_iter->apply_functor.get())) {
			if (iter_style_setter->style_type.type_id == active_style_id) {
				radio_iter->state = bwWidget::STATE_SUNKEN;
			}
			else {
				radio_iter->state = bwWidget::STATE_NORMAL;
			}
		}
	}

	return true;
}

void StyleSetter::operator()()
{
	bwStyle::StyleTypeID style_type_id = style_type.type_id;
	stage.activateStyleID(style_type_id);
	// Deactivate other style radio buttons
	stage.layout->iterateWidgets(StyleButtonsUpdateCb, &style_type_id);
}
