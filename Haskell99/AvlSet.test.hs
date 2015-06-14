import Test.HUnit
import AvlSet
                  
assertEqualIntList :: String -> [Int] -> [Int] -> Assertion
assertEqualIntList = assertEqual

main = 
  let
    tree1 = empty
    tree2 = insert tree1 1
    tree3 = insert tree2 2
    tree4 = insert tree3 3
    tree5 = insert tree4 4
    tree6 = insert tree5 5
    tree7 = insert tree6 6
    tree8 = insert tree7 7
    tree9 = insert tree8 8
    
    test1 = TestCase (assertEqualIntList "toList tree1 should be []               ." ([]               ) (toList tree1))
    test2 = TestCase (assertEqualIntList "toList tree2 should be [1]              ." ([1]              ) (toList tree2))
    test3 = TestCase (assertEqualIntList "toList tree3 should be [1,2]            ." ([1,2]            ) (toList tree3))
    test4 = TestCase (assertEqualIntList "toList tree4 should be [1,2,3]          ." ([1,2,3]          ) (toList tree4))
    test5 = TestCase (assertEqualIntList "toList tree5 should be [1,2,3,4]        ." ([1,2,3,4]        ) (toList tree5))
    test6 = TestCase (assertEqualIntList "toList tree6 should be [1,2,3,4,5]      ." ([1,2,3,4,5]      ) (toList tree6))
    test7 = TestCase (assertEqualIntList "toList tree7 should be [1,2,3,4,5,6]    ." ([1,2,3,4,5,6]    ) (toList tree7))
    test8 = TestCase (assertEqualIntList "toList tree8 should be [1,2,3,4,5,6,7]  ." ([1,2,3,4,5,6,7]  ) (toList tree8))
    test9 = TestCase (assertEqualIntList "toList tree9 should be [1,2,3,4,5,6,7,8]." ([1,2,3,4,5,6,7,8]) (toList tree9))
    
    -- todo - test deletion scenarios
  in
    runTestTT $ TestList [test1,test2,test3,test4,test5,test6,test7,test8,test9]