TEMPLATE = subdirs

SUBDIRS += McIocContainer

SUBDIRS += McIocBoot
McIocBoot.depends += McIocContainer

SUBDIRS += Test
Test.depends += McIocContainer McIocBoot
