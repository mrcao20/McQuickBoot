TEMPLATE = subdirs

SUBDIRS += McYaml \
    McIocContainer \
    McIocBoot \
    McLogQt \
    Examples

#SUBDIRS += McIocBoot
#McIocBoot.depends += McIocContainer

#SUBDIRS += McLogQt
#McLogQt.depends += McIocContainer

#SUBDIRS += Examples
#Examples.depends += McIocContainer McIocBoot McLogQt

CONFIG += ordered
