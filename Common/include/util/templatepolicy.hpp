/*
 * templatepolicy.hpp
 *
 *  Created on: 09-Jul-2016
 *      Author: ankithbti
 */

#ifndef COMMON_INCLUDE_UTIL_TEMPLATEPOLICY_HPP_
#define COMMON_INCLUDE_UTIL_TEMPLATEPOLICY_HPP_

#include <Common.hpp>

namespace  obLib{

template<typename Base, int D>
class PolicyDiscriminator : public Base{

};

template<typename Setter1>
class PolicySelector1 : public PolicyDiscriminator<Setter1, 1>{

};

template<typename Setter1, typename Setter2>
class PolicySelector2 :
		public PolicyDiscriminator<Setter1, 1>,
		public PolicyDiscriminator<Setter2, 2>{

};

template<typename Setter1, typename Setter2, typename Setter3>
class PolicySelector3 :
		public PolicyDiscriminator<Setter1, 1>,
		public PolicyDiscriminator<Setter2, 2>,
		public PolicyDiscriminator<Setter3, 3>{

};

template<typename Setter1, typename Setter2, typename Setter3, typename Setter4>
class PolicySelector4 :
		public PolicyDiscriminator<Setter1, 1>,
		public PolicyDiscriminator<Setter2, 2>,
		public PolicyDiscriminator<Setter3, 3>,
		public PolicyDiscriminator<Setter4, 4>{

};

template<typename Setter1, typename Setter2, typename Setter3, typename Setter4, typename Setter5>
class PolicySelector5 :
		public PolicyDiscriminator<Setter1, 1>,
		public PolicyDiscriminator<Setter2, 2>,
		public PolicyDiscriminator<Setter3, 3>,
		public PolicyDiscriminator<Setter4, 4>,
		public PolicyDiscriminator<Setter5, 5>{

};


#define DECLARE_POLICY_LIST(policylist) \

#define BEGIN_POLICY_DEFAULTS(ploicylist) \
    class policylist##DefaultPolicies { \
    public:

#define DECLARE_PRIMITIVE_DEFAULT(policylist, policy, def)	\
    enum {	\
  policy = def	\
};

#define DECLARE_TYPE_DEFAULT(policylist, policy, def)	\
    typedef def policy;

#define END_POLICY_DEFAULTS(policylist)	\
    };	\
    class policylist##DefaultPolicies;	\
    class policylist##DefaultPolicies;	\
    class policylist##DefaultPolicyArgs : \
    	virtual public policylist##DefaultPolicies	\
    {};


#define DECLARE_PRIMITIVE_POLICY(policylist, policy, type)	\
    template <type T##policy>	\
    class policy##_is : virtual public policylist##DefaultPolicies	\
    {	\
    public:	\
    enum { \
      policy = T##policy \
    };	\
    };

#define DECLARE_TYPE_POLICY(policylist, policy)	\
    template <type T##policy>	\
    class policy##_is : virtual public policylist##DefaultPolicies	\
    {	\
    public:	\
    typedef T##policy policy;	\
    };
}



#endif /* COMMON_INCLUDE_UTIL_TEMPLATEPOLICY_HPP_ */
