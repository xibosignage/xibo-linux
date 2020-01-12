#include "control/widgets/OverlayContainer.hpp"
#include "control/widgets/gtk/FixedContainerGtk.hpp"

class OverlayContainerGtk : public OverlayContainer<BaseFixedContainerGtk<Xibo::OverlayContainer>>
{
protected:
    void setMainChildImpl(const std::shared_ptr<Xibo::Widget>& mainChild) override
    {
        auto& childHandler = handler(mainChild);
        get().add(childHandler);
        childHandler.set_valign(Gtk::ALIGN_CENTER);
        childHandler.set_halign(Gtk::ALIGN_CENTER);
    }

    void removeMainChildImpl(const std::shared_ptr<Xibo::Widget>& /*mainChild*/) override
    {
        get().remove();
    }
};
