#include "MainLayoutView.hpp"

#include <gtkmm/window.h>

MainLayoutView::MainLayoutView(int width, int height) :
    Widget(m_overlayLayout)
{
    m_overlayLayout.signal_get_child_position().
            connect(sigc::mem_fun(*this, &MainLayoutView::onGetRegionPosition), false);

    setSize(width, height);
}

void MainLayoutView::addRegion(const std::shared_ptr<Widget>& child, int x, int y, int z)
{
    m_overlayLayout.add_overlay(child->get());

    WidgetInfo info{child, child->width(), child->height(), x, y, z};
    m_regions.emplace_back(info);

    sortRegionsByZindex();
    reorderRegions();
}

void MainLayoutView::sortRegionsByZindex()
{
    std::stable_sort(m_regions.begin(), m_regions.end(), [=](const auto& first, const auto& second){
        return first.z < second.z;
    });
}

void MainLayoutView::reorderRegions()
{
    for(std::size_t i = 0; i != m_regions.size(); ++i)
    {
        auto&& widget = m_regions[i].widget;
        m_overlayLayout.reorder_overlay(widget->get(), static_cast<int>(i));
    }
}

void MainLayoutView::reorderRegion(const std::shared_ptr<Widget>& child, int z)
{
    m_overlayLayout.reorder_overlay(child->get(), z);
}

void MainLayoutView::addBackground(const std::shared_ptr<Widget>& mainChild)
{
    m_overlayLayout.add(mainChild->get());

    m_mainChild = mainChild;
}

bool MainLayoutView::hasBackground() const
{
    return m_mainChild != nullptr;
}

void MainLayoutView::showAll()
{
    Widget::showAll();
    m_mainChild->showAll();

    for(auto&& childInfo : m_regions)
    {
        childInfo.widget->showAll();
    }
}

void MainLayoutView::setSize(int width, int height)
{
    m_overlayLayout.set_size_request(width, height);
    updateOffsets();
}

void MainLayoutView::scale(double scaleX, double scaleY)
{
    Widget::scale(scaleX, scaleY);
    m_mainChild->scale(scaleX, scaleY);
    updateOffsets();
    scaleRegions(scaleX, scaleY);
}

void MainLayoutView::updateOffsets()
{
    Gtk::Window* parent = static_cast<Gtk::Window*>(m_overlayLayout.get_toplevel());
    if(parent->get_is_toplevel())
    {
        int width, height;
        parent->get_default_size(width, height);

        m_xOffset = (width - this->width())/2;
        m_yOffset = (height - this->height())/2;
    }
}

void MainLayoutView::scaleRegions(double scaleX, double scaleY)
{
    for(auto&& childInfo : m_regions)
    {
        childInfo.widget->scale(scaleX, scaleY);

        childInfo.x *= scaleX;
        childInfo.y *= scaleY;
    }
}

Gtk::Overlay& MainLayoutView::get()
{
    return m_overlayLayout;
}

bool MainLayoutView::onGetRegionPosition(Gtk::Widget* widget, Gdk::Rectangle& alloc)
{
    auto it = findRegion(widget);
    if(it != m_regions.end())
    {
        WidgetInfo info = *it;

        alloc.set_x(m_xOffset + info.x);
        alloc.set_y(m_yOffset + info.y);
        alloc.set_width(info.width);
        alloc.set_height(info.height);
        return true;
    }
    return false;
}

MainLayoutView::WidgetsWithInfo::iterator MainLayoutView::findRegion(Gtk::Widget* widget)
{
    return std::find_if(m_regions.begin(), m_regions.end(), [widget](const auto& child){
        return &child.widget->get() == widget;
    });
}
