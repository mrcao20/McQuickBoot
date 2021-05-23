#include "McWidgetIoc/BuiltInBeanBuilder/McBuiltInBeanContainer.h"

#include "McWidgetIoc/BuiltInBeanBuilder/IMcBuiltInBeanBuilderFactory.h"

namespace {
using BuilderFactoryType = QHash<QString, IMcBuiltInBeanBuilderFactoryPtr>;
MC_GLOBAL_STATIC(BuilderFactoryType, staticBuilderFactory)
} // namespace

void McBuiltInBeanContainer::setBuilderFactory(
    const QString &className, IMcBuiltInBeanBuilderFactoryConstPtrRef factory) noexcept
{
    staticBuilderFactory->insert(className, factory);
}

IMcBuiltInBeanBuilderPtr McBuiltInBeanContainer::getBuilder(const QString &className) noexcept
{
    if (!staticBuilderFactory->contains(className)) {
        return IMcBuiltInBeanBuilderPtr();
    }
    return staticBuilderFactory->value(className)->create();
}

#include <QLabel>
#include <QStackedWidget>

MC_STATIC()
MC_REGISTER_LIST_CONVERTER(QList<QWidget *>)
{
    auto func = [](QStackedWidget *w, const QString &name, const QVariant &val) {
        if (name == "widgets") {
            auto widgets = val.value<QList<QWidget *>>();
            for (auto widget : widgets) {
                w->addWidget(widget);
            }
        }
    };
    McBuiltInBeanContainer::setBuilderFactory<QStackedWidget>(func);
}
{
    auto func = [](QLabel *w, const QString &name, const QVariant &val) {
        w->setProperty(name.toLocal8Bit(), val);
    };
    McBuiltInBeanContainer::setBuilderFactory<QLabel>(func);
}
MC_STATIC_END
