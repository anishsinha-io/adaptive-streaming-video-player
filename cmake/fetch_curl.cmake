set(CURL_VERSION curl-8_16_0)
find_package(CURL QUIET)
if (NOT CURL_FOUND)
  include(FetchContent)
  FetchContent_Declare(
    curl
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/curl/curl/archive/refs/tags/${CURL_VERSION}.tar.gz
  )
  FetchContent_GetProperties(curl)
  if (NOT curl_POPULATED)
    set(FETCHCONTENT_QUIET NO)
    FetchContent_MakeAvailable(curl)
  endif()
endif()
