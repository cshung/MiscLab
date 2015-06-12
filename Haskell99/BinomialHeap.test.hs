import Test.HUnit
import BinomialHeap

main =
  let
    q01 :: BinomialHeap Int

    q01 = empty
    q02 = insert q01 3
    q03 = insert q02 0
    q04 = insert q03 6
    q05 = insert q04 2
    q06 = insert q05 4
    q07 = insert q06 7
    q08 = insert q07 0
    q09 = insert q08 0

    Just (v1, q10) = deleteMin q09
    Just (v2, q11) = deleteMin q10
    Just (v3, q12) = deleteMin q11
    Just (v4, q13) = deleteMin q12
    Just (v5, q14) = deleteMin q13
    Just (v6, q15) = deleteMin q14
    Just (v7, q16) = deleteMin q15
    Just (v8, q17) = deleteMin q16

    test1 = TestCase (assertEqual "v1 should be 0." (0) (v1))
    test2 = TestCase (assertEqual "v2 should be 0." (0) (v2))
    test3 = TestCase (assertEqual "v3 should be 0." (0) (v3))
    test4 = TestCase (assertEqual "v4 should be 2." (2) (v4))
    test5 = TestCase (assertEqual "v5 should be 3." (3) (v5))
    test6 = TestCase (assertEqual "v6 should be 4." (4) (v6))
    test7 = TestCase (assertEqual "v7 should be 6." (6) (v7))
    test8 = TestCase (assertEqual "v8 should be 7." (7) (v8))

  in
    runTestTT $ TestList [test1,test2,test3,test4,test5,test6,test7,test8]