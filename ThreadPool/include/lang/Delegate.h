#ifndef _LANG_DELEGATE_H
#define _LANG_DELEGATE_H

#include "xnet/xnetdata.h"
#include "xnet/titlelist.h"

using namespace xnet;
namespace lang {

struct handle {
	virtual void invoke(TXNetData *XnetData) = 0;
	virtual ~handle() {}
};

template <class _Ty>
struct delegate : handle {
	typedef void (_Ty::*pfun)(TXNetData *XnetData);
	typedef _Ty  *pcls;

	delegate(pfun the_fun, pcls the_cls)
		: fun_(the_fun), cls_(the_cls)
	{}

	virtual void invoke(TXNetData *XnetData) { (cls_->*fun_)(XnetData); }

private:

	pfun fun_;
	pcls cls_;
};

}
#endif //_FRAME_DELEGATE