#ifndef THREADSAFE_QUEUE_HPP
#define THREADSAFE_QUEUE_HPP

#include <queue>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>

template< typename T >
struct threadsafe_queue
{
private:

    typedef boost::lock_guard< boost::mutex > lock_t;
    typedef boost::unique_lock< boost::mutex > unique_lock_t;

public:

    // default constructor
    threadsafe_queue() {}

    // copy constructor
    threadsafe_queue( const threadsafe_queue& rhs )
    {
        lock_t l( rhs.mutex );

        data = rhs.data;
    }

    // push
    void push( T new_value )
    {
        lock_t l( mutex );

        data.push( new_value );
        condition.notify_one();
    }

    //try_pop
    bool try_pop( T& value )
    {
        lock_t l( mutex );

        if( data.empty() )
        {
            return false;
        }

        value = data.front();
        data.pop();

        return true;
    }

    boost::shared_ptr< T > try_pop()
    {
        lock_t l( mutex );

        if( data.empty() )
        {
            return boost::shared_ptr< T >();
        }

        boost::shared_ptr< T > ret = boost::make_shared<T>( data.front() );
        data.pop();

        return ret;
    }

    //wait_and_pop
    void wait_and_pop( T& value )
    {
        unique_lock_t l( mutex );

        condition.wait( l
                      , [this] () { return ( this->data.empty() == false ); }
                      );

        value = data.front();
        data.pop();
    }

    boost::shared_ptr< T > wait_and_pop()
    {
        unique_lock_t l( mutex );

        condition.wait( l
                      , [&] () -> bool
                        {
                            return ( this->data.empty() == false );
                        }
                      );
        
        boost::shared_ptr< T > ret = boost::make_shared<T>( data.front() );
        data.pop();

        return ret;
    }

    // empty
    bool empty() const
    {
        lock_t l( mutex );

        return data.empty();
    }

private:

    threadsafe_queue& operator=( const threadsafe_queue& );        

private:

    std::queue<T> data;

    boost::mutex mutex;
    boost::condition_variable condition;

};


#endif // THREADSAFE_QUEUE_HPP