#pragma once

#include <Images/Exceptions.H>

#define UnexpectedError() UnexpectedException(__PRETTY_FUNCTION__,__FILE__,__LINE__)
