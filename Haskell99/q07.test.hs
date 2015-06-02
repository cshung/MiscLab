import Test.HUnit
import Q07

test1 = TestCase (assertEqual "flatten (Elem 5)                               should be [5]."     ([5])     (flatten (Elem 5))                              )
test2 = TestCase (assertEqual "flatten (List [Elem 1, Elem 2])                should be [1,2]."   ([1,2])   (flatten (List [Elem 1, Elem 2]))               )
test3 = TestCase (assertEqual "flatten (List [List [Elem 1, Elem 2], Elem 3]) should be [1,2,3]." ([1,2,3]) (flatten (List [List [Elem 1, Elem 2], Elem 3])))
test4 = TestCase (assertEqual "flatten (List [Elem 1, List [Elem 2, Elem 3]]) should be [1,2,3]." ([1,2,3]) (flatten (List [Elem 1, List [Elem 2, Elem 3]])))

main = runTestTT $ TestList [test1,test2,test3,test4]