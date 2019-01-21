using Xunit;

namespace BoyerMoore.Tests
{
    public class BooreMooreTests
    {
        [Fact]
        public void HelloWorld()
        {
            Assert.Equal(new int[] { 6 }, BoyerMoore.IndexOf("Hello World", "World"));
        }

        [Fact]
        public void MatchSuffixAndShift()
        {
            Assert.Equal(new int[] { 2 }, BoyerMoore.IndexOf("pppqpq", "pqpq"));
        }

        [Fact]
        public void MatchOverlapPattern()
        {
            Assert.Equal(new int[] { 0, 2 }, BoyerMoore.IndexOf("pqpqpq", "pqpq"));
        }

        [Fact]
        public void Fail1()
        {
            //                                                012345
            //                                                     BC..ABC
            Assert.Equal(new int[] { 5 }, BoyerMoore.IndexOf("BC.xABC..ABC", "BC..ABC"));
        }

        [Fact]
        public void Fail2()
        {
            //                                                012345
            //                                                     ..ABC
            Assert.Equal(new int[] { 5 }, BoyerMoore.IndexOf("x.ABC..ABC", "..ABC"));
        }
    }
}
