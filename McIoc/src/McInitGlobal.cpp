#include "McIoc/BeanFactory/impl/McBeanConnector.h"
#include "McIoc/BeanFactory/impl/McBeanEnum.h"
#include "McIoc/BeanFactory/impl/McBeanPlaceholder.h"
#include "McIoc/BeanFactory/impl/McBeanReference.h"

MC_STATIC()
MC_REGISTER_BEAN_FACTORY(McBeanEnum)
MC_REGISTER_BEAN_FACTORY(McBeanReference)
MC_REGISTER_BEAN_FACTORY(McBeanPlaceholder)
MC_REGISTER_BEAN_FACTORY(McBeanConnector)
MC_STATIC_END
