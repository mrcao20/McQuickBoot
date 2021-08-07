TEMPLATE = subdirs

SUBDIRS += McIoc \
    McWidgetIoc \
    McLogQt \
    McOrm \
    McQuickBoot \
    Examples

#SUBDIRS += McQuickBoot
#McQuickBoot.depends += McIoc

#SUBDIRS += McLogQt
#McLogQt.depends += McIoc

#SUBDIRS += Examples
#Examples.depends += McIoc McQuickBoot McLogQt

CONFIG += ordered
