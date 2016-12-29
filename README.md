### Fox C++ library

mutex.hpp provide a very useful std mutex wrapper named GeneralMutex.

For example:

fox:: GeneralMutex<> //can use as std::mutex ,ie : NotTimed, NotResursive, NotShared

fox:: GeneralMutex < fox :: Recursive, fox :: Timed > // can use as std::recursive_timed_mutex

fox:: GeneralMutex < fox :: Timed , fox :: Recursive> // the same


fox:: GeneralMutex< fox:: timed, fox:: Shared>  // std::shared_timed_mutex

fox:: GeneralMutex< fox:: timed, fox:: NotResursive, fox:: Shared>  // the same


Three policy are supported, timed, recursive and shared. You can provide the policy is any order.

