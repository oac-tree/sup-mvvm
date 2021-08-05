// ************************************************************************** //
//
//  Sequencer GUI prototype
//
// ************************************************************************** //

#ifndef TESTUTILS_H
#define TESTUTILS_H

//! @file test_utils.h
//! Collection of utility functions for various unit tests.

#include <algorithm>
#include <memory>
#include <vector>

namespace ModelView
{
class ViewItem;
}

//! Various common utils for unit tests.

namespace TestUtils
{
//! Deletes items in the container and cleans container afterwards.

template <typename T>
void clean_items(T& items)
{
  for (auto item : items) delete item;
  items.clear();
}

//! Creates vector of unique_ptr of given type.

template <typename B, typename D>
auto create_row(int ncolumns)
{
  std::vector<std::unique_ptr<B>> result;
  for (int i = 0; i < ncolumns; ++i) result.emplace_back(std::make_unique<D>());
  return result;
}

//! Creates vector of pointers from vector of unique_ptr.

template <typename T>
auto create_pointers(const std::vector<std::unique_ptr<T>>& vec)
{
  std::vector<T*> result;
  std::transform(vec.begin(), vec.end(), std::back_inserter(result),
                 [](auto& x) { return x.get(); });
  return result;
}

template <typename T>
auto CreateTestData()
{
  auto uptr = std::make_unique<T>();
  auto raw_ptr = uptr.get();
  return std::make_pair(std::move(uptr), raw_ptr);
}

//! Creates string representing XML document. User provided body will be enclosed between standard
//! header and footer.

std::string CreateXMLDocumentString(const std::string& body);

}  // namespace TestUtils

#endif
