TEMPLATE = subdirs

SUBDIRS += McIocContainer

SUBDIRS += McIocBoot
McIocBoot.depends += McIocContainer

SUBDIRS += McLogQt
McLogQt.depends += McIocContainer

SUBDIRS += Examples
Examples.depends += McIocContainer McIocBoot McLogQt
