#include <gui/common/FrontendApplication.hpp>

#include <new>
#include <gui/common/FrontendHeap.hpp>
#include <touchgfx/transitions/NoTransition.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Texts.hpp>
#include <touchgfx/hal/HAL.hpp>
#include<platform/driver/lcd/LCD24bpp.hpp>
#include <gui/inputsalescreen_screen/InputSaleScreenView.hpp>
#include <gui/inputsalescreen_screen/InputSaleScreenPresenter.hpp>
#include <gui/operationpumpscreen_screen/OperationPumpScreenPresenter.hpp>
#include <gui/operationpumpscreen_screen/OperationPumpScreenView.hpp>

FrontendApplication::FrontendApplication(Model& m, FrontendHeap& heap)
    : FrontendApplicationBase(m, heap),
	  transitionCallback_custom(),
	  frontendHeap_custom(heap),
	  model_custom(m)
{
    touchgfx::HAL::getInstance()->setDisplayOrientation(touchgfx::ORIENTATION_LANDSCAPE);
    touchgfx::Texts::setLanguage(GB);
    reinterpret_cast<touchgfx::LCD24bpp&>(touchgfx::HAL::lcd()).enableTextureMapperAll();
}

void FrontendApplication::gotoInputSaleScreenScreenCoverTransitionSouth()
{
	transitionCallback_custom = touchgfx::Callback<FrontendApplication>(this, &FrontendApplication::gotoInputSaleScreenScreenCoverTransitionSouthImpl);
    pendingScreenTransitionCallback = &transitionCallback_custom;
}

void FrontendApplication::gotoInputSaleScreenScreenCoverTransitionSouthImpl()
{
    touchgfx::makeTransition<InputSaleScreenView, InputSaleScreenPresenter, touchgfx::CoverTransition<SOUTH>, Model >(&currentScreen, &currentPresenter, frontendHeap_custom, &currentTransition, &model_custom);
}

void FrontendApplication::gotoOperationPumpScreenCoverTransitionSouth()
{
	transitionCallback_custom = touchgfx::Callback<FrontendApplication>(this, &FrontendApplication::gotoOperationPumpScreenCoverTransitionSouthImpl);
}

void FrontendApplication::gotoOperationPumpScreenCoverTransitionSouthImpl()
{
	touchgfx::makeTransition<OperationPumpScreenView, OperationPumpScreenPresenter, touchgfx::CoverTransition<SOUTH>, Model >(&currentScreen, &currentPresenter, frontendHeap_custom, &currentTransition, &model_custom);
}
