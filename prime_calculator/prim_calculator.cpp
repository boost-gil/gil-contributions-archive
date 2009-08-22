// prim_calculator.cpp : Defines the entry point for the console application.
//

#include <algorithm>
#include <cmath>
#include <numeric>

#include <boost/mapreduce.hpp>

namespace prime_calculator
{

bool is_prime( std::size_t number )
{
    if( number == 2 )
        return true;
        
    if( number % 2 == 0 )
        return false;

    long n = static_cast<long>( number );

    if( number == 0 || number == 1 )
        return false;

    n = std::abs( n );
    std::size_t sqrt_number = static_cast< std::size_t >( std::sqrt( static_cast< double >( n )));

    for( std::size_t i = 2; i <= sqrt_number; i++ )
    {
        if( n % i == 0 )
            return false;
    }

    return true;
}

template< typename MapTask >
class number_source : boost::noncopyable
{
public:

    number_source( std::size_t start
                 , std::size_t end
                 )
    : _start  ( start )
    , _end    ( end   )
    , _current( start )
    {}

    const bool setup_key( typename MapTask::key_type& key ) const
    {
        if( _current < _end )
        {
            key = _current;
            return true;
        }
        else
        {
            return false;
        }
    }

    const bool get_data( typename MapTask::key_type&   key
                       , typename MapTask::value_type& value
                       )
    {
        if( _current < _end )
        {
            value = _current;
            _current++;

            return true;
        }
        else
        {
            return false;
        }
    }

private:

    std::size_t _start;
    std::size_t _end;
    std::size_t _current;
};

struct map_task : public boost::mapreduce::map_task< std::size_t // key_type
                                                   , std::size_t // value_type
                                                   >
{
    template<typename Runtime>
    static void map( Runtime& runtime
                   , const key_type& //key
                   , value_type&     value
                   )
    {
        runtime.emit_intermediate( is_prime( value ), value );
    }
};

struct reduce_task : public boost::mapreduce::reduce_task< bool        // key_type
                                                         , std::size_t // value_type
                                                         >
{
    template< typename Runtime
            , typename It
            >
    static void reduce( Runtime&        runtime
                      , const key_type& key
                      , It              it
                      , const It        ite
                      )
    {
        if( key )
        {
            for_each( it
                    , ite
                    , boost::bind( &Runtime::emit
                                 , boost::ref( runtime )
                                 , true
                                 , _1
                                 )
                    );
        }
    }
};

typedef boost::mapreduce::job< prime_calculator::map_task
                             , prime_calculator::reduce_task
                             , boost::mapreduce::null_combiner
                             , prime_calculator::number_source< prime_calculator::map_task >
                             , boost::mapreduce::intermediates::in_memory< prime_calculator::map_task
                                                                         , prime_calculator::reduce_task
                                                                         >
                            , boost::mapreduce::intermediates::reduce_file_output< prime_calculator::map_task
                                                                                 , prime_calculator::reduce_task
                                                                                 >
                            > job;

/*
typedef
boost::mapreduce::job< prime_calculator::map_task
                     , prime_calculator::reduce_task
                     , boost::mapreduce::null_combiner
                     , prime_calculator::number_source< prime_calculator::map_task >
                     > job;
*/

} // namespace prime_calculator

int main( int argc, char* argv[])
{
    boost::mapreduce::specification spec;

    boost::mapreduce::results result;
    prime_calculator::job::datasource_type datasource( 0, 1000000 );

    spec.map_tasks    = 0;
    spec.reduce_tasks = std::max( 1U, boost::thread::hardware_concurrency() );

    std::cout << "\nRunning Parallel Prime_Calculator MapReduce..." << std::endl;
    prime_calculator::job job( datasource, spec );
    job.run< boost::mapreduce::schedule_policy::cpu_parallel<prime_calculator::job > >( result );
    std::cout << "\nMapReduce Finished." << std::endl;

    std::cout << std::endl << "\nMapReduce statistics:";
    std::cout << "\n  MapReduce job runtime                     : " << result.job_runtime << " seconds, of which...";
    std::cout << "\n    Map phase runtime                       : " << result.map_runtime << " seconds";
    std::cout << "\n    Reduce phase runtime                    : " << result.reduce_runtime << " seconds";
    std::cout << "\n\n  Map:";
    std::cout << "\n    Total Map keys                          : " << result.counters.map_keys_executed;
    std::cout << "\n    Map keys processed                      : " << result.counters.map_keys_completed;
    std::cout << "\n    Map key processing errors               : " << result.counters.map_key_errors;
    std::cout << "\n    Number of Map Tasks run (in parallel)   : " << result.counters.actual_map_tasks;
    std::cout << "\n    Fastest Map key processed in            : " << *std::min_element(result.map_times.begin(), result.map_times.end()) << " seconds";
    std::cout << "\n    Slowest Map key processed in            : " << *std::max_element(result.map_times.begin(), result.map_times.end()) << " seconds";
    std::cout << "\n    Average time to process Map keys        : " << std::accumulate(result.map_times.begin(), result.map_times.end(), boost::posix_time::time_duration()) / result.map_times.size() << " seconds";

    std::cout << "\n\n  Reduce:";
    std::cout << "\n    Total Reduce keys                       : " << result.counters.reduce_keys_executed;
    std::cout << "\n    Reduce keys processed                   : " << result.counters.reduce_keys_completed;
    std::cout << "\n    Reduce key processing errors            : " << result.counters.reduce_key_errors;
    std::cout << "\n    Number of Reduce Tasks run (in parallel): " << result.counters.actual_reduce_tasks;
    std::cout << "\n    Number of Result Files                  : " << result.counters.num_result_files;
    if (result.reduce_times.size() > 0)
    {
        std::cout << "\n    Fastest Reduce key processed in         : " << *std::min_element(result.reduce_times.begin(), result.reduce_times.end()) << " seconds";
        std::cout << "\n    Slowest Reduce key processed in         : " << *std::max_element(result.reduce_times.begin(), result.reduce_times.end()) << " seconds";
        std::cout << "\n    Average time to process Reduce keys     : " << std::accumulate(result.reduce_times.begin(), result.reduce_times.end(), boost::posix_time::time_duration()) / result.map_times.size() << " seconds";
    }

    std::cout << std::endl;

	return 0;
}
