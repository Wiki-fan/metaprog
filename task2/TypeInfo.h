#pragma once

#include <typeinfo>
#include <cassert>


class TypeInfo
{
 public:
    // needed for containers
    TypeInfo()
    {
        class Nil
        {
        };
        pInfo_ = &typeid( Nil );
        assert( pInfo_ );
    }

    // non-explicit
    TypeInfo( const std::type_info& ti ) : pInfo_( &ti )
    {
        assert( pInfo_ );
    }

    // Access for the wrapped std::type_info
    const std::type_info& Get() const
    {
        assert( pInfo_ );
        return *pInfo_;
    }

    // Compatibility functions
    bool before( const TypeInfo& rhs ) const
    {
        assert( pInfo_ );
        // type_info::before return type is int in some VC libraries
        return pInfo_->before( *rhs.pInfo_ ) != 0;
    }

    const char* name() const
    {
        assert( pInfo_ );
        return pInfo_->name();
    }

 private:
    const std::type_info* pInfo_;
};

inline bool operator==( const TypeInfo& lhs, const TypeInfo& rhs )
// type_info::operator== return type is int in some VC libraries
{
    return ( lhs.Get() == rhs.Get() ) != 0;
}

inline bool operator<( const TypeInfo& lhs, const TypeInfo& rhs )
{
    return lhs.before( rhs );
}

inline bool operator!=( const TypeInfo& lhs, const TypeInfo& rhs )
{
    return !( lhs == rhs );
}

inline bool operator>( const TypeInfo& lhs, const TypeInfo& rhs )
{
    return rhs < lhs;
}

inline bool operator<=( const TypeInfo& lhs, const TypeInfo& rhs )
{
    return !( lhs > rhs );
}

inline bool operator>=( const TypeInfo& lhs, const TypeInfo& rhs )
{
    return !( lhs < rhs );
}

