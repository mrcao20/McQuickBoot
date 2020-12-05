#include "TestVo.h"

#include "Article.h"

MC_INIT(TestVo)
MC_REGISTER_BEAN_FACTORY(TestVo)
MC_REGISTER_CONTAINER_CONVERTER(QList<ArticlePtr>)
MC_INIT_END
