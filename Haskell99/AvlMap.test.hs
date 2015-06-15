import Test.HUnit
import AvlMap

assertEqualBoolList :: String -> [Bool] -> [Bool] -> Assertion
assertEqualBoolList = assertEqual

assertEqualCharList :: String -> [Char] -> [Char] -> Assertion
assertEqualCharList = assertEqual

main =
  let
    tree0          = empty
    tree1          = insert tree0 100 'a'
    tree2          = insert tree1 200 'b'
    tree3          = insert tree2 150 'c'
    (_,tree4)      = deleteByKey tree3 100
    c01            = containsKey tree0 100
    c02            = containsKey tree1 100
    c03            = containsKey tree2 100
    c04            = containsKey tree3 100
    c05            = containsKey tree0 150
    c06            = containsKey tree1 150
    c07            = containsKey tree2 150
    c08            = containsKey tree3 150
    c09            = containsKey tree0 200
    c10            = containsKey tree1 200
    c11            = containsKey tree2 200
    c12            = containsKey tree3 200
    v1             = getValueByKey tree3 100
    v2             = getValueByKey tree3 150
    v3             = getValueByKey tree3 200
    test1          = TestCase (assertEqualBoolList "Validating containsKey." ([False, True, True, True, False, False, False, True, False, False, True, True]) ([c01, c02, c03, c04, c05, c06, c07, c08, c09, c10, c11, c12]))
    test2          = TestCase (assertEqualCharList "Validating valueByKey." (['a', 'c', 'b']) ([v1, v2, v3]))
  in
    runTestTT $ TestList [test1,test2]