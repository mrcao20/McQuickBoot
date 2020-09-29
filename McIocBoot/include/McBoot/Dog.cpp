#include "Dog.h"

MC_INIT(Dog)
MC_REGISTER_BEAN_FACTORY(Dog)
MC_INIT_END

Dog::Dog(QObject *parent) : QObject(parent)
{
    
}
