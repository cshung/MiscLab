import Test.HUnit
import Q28

assertEqualIntListList :: String -> [[Int]] -> [[Int]] -> Assertion
assertEqualIntListList = assertEqual

test01 = TestCase (assertEqualIntListList "lsort []               should be []             ." ([]             ) (lsort []              ))
test02 = TestCase (assertEqualIntListList "lsort [[1]]            should be [[1]]          ." ([[1]]          ) (lsort [[1]]           ))
test03 = TestCase (assertEqualIntListList "lsort [[1],[2]]        should be [[1],[2]]      ." ([[1],[2]]      ) (lsort [[1],[2]]       ))
test04 = TestCase (assertEqualIntListList "lsort [[1,2],[3]]      should be [[3],[1,2]]    ." ([[3],[1,2]]    ) (lsort [[1,2],[3]]     ))
test05 = TestCase (assertEqualIntListList "lsort [[1,2],[3],[4]]  should be [[3],[4],[1,2]]." ([[3],[4],[1,2]]) (lsort [[1,2],[3],[4]] ))
test06 = TestCase (assertEqualIntListList "lfsort []              should be []             ." ([]             ) (lfsort []             ))
test07 = TestCase (assertEqualIntListList "lfsort [[1]]           should be [[1]]          ." ([[1]]          ) (lfsort [[1]]          ))
test08 = TestCase (assertEqualIntListList "lfsort [[1],[2]]       should be [[1],[2]]      ." ([[1],[2]]      ) (lfsort [[1],[2]]      ))
test09 = TestCase (assertEqualIntListList "lfsort [[1,2],[3]]     should be [[1,2],[3]]    ." ([[1,2],[3]]    ) (lfsort [[1,2],[3]]    ))
test10 = TestCase (assertEqualIntListList "lfsort [[1,2],[3],[4]] should be [[1,2],[3],[4]]." ([[1,2],[3],[4]]) (lfsort [[1,2],[3],[4]]))

main = runTestTT $ TestList [test01,test02,test03,test04,test05,test06,test07,test08,test09,test10]