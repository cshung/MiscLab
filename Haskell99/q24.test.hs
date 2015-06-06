import Test.HUnit
import Q24

test1 = TestCase (assertEqual "diffSelectTest 0 2 should be Right Selecting less than one element is not supported    ." (Right "Selecting less than one element is not supported"    ) (diffSelectTest 0 2))
test2 = TestCase (assertEqual "diffSelectTest 1 2 should be Left [1]                                                  ." (Left [1]                                                    ) (diffSelectTest 1 2))
test3 = TestCase (assertEqual "diffSelectTest 2 2 should be Left [1,2]                                                ." (Left [1,2]                                                  ) (diffSelectTest 2 2))
test4 = TestCase (assertEqual "diffSelectTest 3 2 should be Right Selecting more than one numElements is not supported." (Right "Selecting more than one numElements is not supported") (diffSelectTest 3 2))


main = runTestTT $ TestList [test1,test2,test3,test4]