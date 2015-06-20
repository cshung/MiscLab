namespace SuffixTreePrototype
{
    using System;
    using System.Collections.Generic;
    using System.Diagnostics;
    using System.Text;

    internal class SuffixTree
    {
        // Turn these on to understand more how its internal works
        private static bool assertOn = false;
        private static bool loggingOn = false;

        private int currentPhaseEnd;
        private SuffixTreeNode rootNode;
        private SuffixTreeNode fullStringLeaf;
        private SuffixTreeNode lastInternalNode;
        private SuffixTreeNode lastExtensionSearchNode;
        private string text;

        // debugging only fields
        private int extensionCount;
        private int lastInternalNodeBirthday;

        private SuffixTree(string text)
        {
            // Avoid direct construction
            this.text = text;
        }

        public static SuffixTree Build(string text)
        {
            SuffixTree result = new SuffixTree(text);
            result.BuildTree(text);
            return result;
        }

        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();
            this.rootNode.BuildString(this.text, sb, 0);
            return sb.ToString();
        }

        private void BuildTree(string text)
        {
            // Step 1: Build initial tree
            this.rootNode = new SuffixTreeNode(0);
            this.fullStringLeaf = new SuffixTreeNode(1);
            SuffixTreeLink nodeToFullStringLeafLink = new SuffixTreeLink(this) { Parent = this.rootNode, Start = 0, Child = this.fullStringLeaf };
            this.rootNode.Links.Add(text[0], nodeToFullStringLeafLink);
            this.fullStringLeaf.ParentLink = nodeToFullStringLeafLink;

            // debugging
            this.extensionCount = 0;

            // Step 2: Basic extension loop
            int startIndex = 1;
            this.currentPhaseEnd = 1;
            this.lastExtensionSearchNode = this.rootNode;

            for (int endIndex = 2; endIndex <= text.Length; endIndex++)
            {
                if (loggingOn)
                {
                    Console.WriteLine("Begin phase " + endIndex + " with start index = " + startIndex);
                }
                this.currentPhaseEnd++;
                for (; startIndex < endIndex; startIndex++)
                {
                    if (this.Extend(startIndex, endIndex))
                    {
                        // Short circuit to the next phase if rule 3 ever applied
                        break;
                    }
                }
            }

            if (assertOn)
            {
                this.ValidateSuffixLinks(this.rootNode);
            }
        }

        // startIndex, endIndex are 0 based [) index to the string to be inserted to the current implicit suffix tree
        private bool Extend(int startIndex, int endIndex)
        {
            this.extensionCount++;

            int insertingTextLength = endIndex - startIndex;
            int searchingTextLength = insertingTextLength - 1;

            // Step 1: Search for the end of the searchingText
            //
            // The search result could be 
            // nodeCursor == rootNode, in this case the searchingText is the empty string
            // followingLink != null and 0 < linkCursor <= followingLink.Length(), that represents the exclusive end index of a pointer on the link
            // note that if linkCursor == followingLink.Length(), it means we are at the followingLink.child node
            //
            // We decided to use followingLink != null && linkCursor == followingLink.Length() to represent the cursor is at the next node is because we need 
            // to manipulate the link at times (e.g. extending it in leaf case)
            SuffixTreeNode nodeCursor;
            SuffixTreeLink followingLink;
            int linkCursor;
            this.SearchPrefix(startIndex, searchingTextLength, out nodeCursor, out followingLink, out linkCursor);

            // Step 2: Extend the tree by the last character
            int appliedRuleNumber = this.ExtendByLastCharacter(insertingTextLength, endIndex, nodeCursor, followingLink, linkCursor);
            if (loggingOn)
            {
                Console.WriteLine("Processing " + this.text.Substring(startIndex, endIndex - startIndex) + " used rule " + appliedRuleNumber);
            }
            return appliedRuleNumber == 3;
        }

        private void SearchPrefix(int startIndex, int searchingTextLength, out SuffixTreeNode nodeCursor, out SuffixTreeLink followingLink, out int linkCursor)
        {
            nodeCursor = this.rootNode;
            followingLink = null;
            linkCursor = -1;

            if (startIndex == 0)
            {
                // For the first extension - it is always extending the full string leaf node
                followingLink = this.fullStringLeaf.ParentLink;
                linkCursor = followingLink.Length();
            }
            else
            {
                SuffixTreeNode startNode = this.rootNode;
                Debug.Assert(this.lastExtensionSearchNode != null, "Last extension should have searched something");
                if (this.lastExtensionSearchNode != this.rootNode)
                {
                    Debug.Assert(this.lastExtensionSearchNode.ParentLink != null, "A node should have a parent link if it is not a root node");
                    Debug.Assert(this.lastExtensionSearchNode.ParentLink.Parent != null, "A link should have a parent node");
                    if (this.lastExtensionSearchNode.SuffixLink == null)
                    {
                        if (this.lastExtensionSearchNode.ParentLink.Parent != this.rootNode)
                        {
                            Debug.Assert(this.lastExtensionSearchNode.ParentLink.Parent.SuffixLink != null, "If the lastExtensionSearchNode does not have a suffix link, then its parent is either a root of have a suffix link!");
                            startNode = this.lastExtensionSearchNode.ParentLink.Parent.SuffixLink;
                        }
                    }
                    else
                    {
                        startNode = this.lastExtensionSearchNode.SuffixLink;
                    }
                }

                int remainingSearchingTextLength = searchingTextLength;
                int textCursor = startIndex;

                // Short-circuitting by suffix link
                remainingSearchingTextLength -= startNode.StringDepth;
                textCursor += startNode.StringDepth;
                nodeCursor = startNode;

                while (remainingSearchingTextLength > 0)
                {
                    char currentCharacter = this.text[textCursor];
                    if (followingLink == null)
                    {
                        followingLink = nodeCursor.Links[currentCharacter];
                        nodeCursor = null;
                        linkCursor = 0;
                    }
                    else
                    {
                        if (linkCursor == followingLink.Length())
                        {
                            linkCursor = -1;
                            nodeCursor = followingLink.Child;
                            followingLink = null;
                        }
                        else
                        {
                            int move = Math.Min(followingLink.Length(), remainingSearchingTextLength);
                            if (assertOn)
                            {
                                string movedText = this.text.Substring(textCursor, move);
                                string movedLink = this.text.Substring(followingLink.Start, move);
                                Debug.Assert(string.Equals(movedText, movedLink), "We should be skipping over the right text");
                            }

                            textCursor += move;
                            remainingSearchingTextLength -= move;
                            linkCursor += move;
                        }
                    }
                }
            }
        }

        private int ExtendByLastCharacter(int insertingTextLength, int endIndex, SuffixTreeNode nodeCursor, SuffixTreeLink followingLink, int linkCursor)
        {
            int appliedRuleNumber = -1;
            char characterToExtend = this.text[endIndex - 1];
            SuffixTreeNode newlyCreatedInternalNode = null;
            if (followingLink != null)
            {
                if (linkCursor == followingLink.Length())
                {
                    if (followingLink.Child.Links.Count == 0)
                    {
                        // Rule 1 - we have reached a leaf - extend the leaf, done
                        followingLink.Child.StringDepth++;
                        this.lastExtensionSearchNode = followingLink.Child.ParentLink.Parent;

                        appliedRuleNumber = 1;
                    }
                    else
                    {
                        if (followingLink.Child.Links.ContainsKey(characterToExtend))
                        {
                            appliedRuleNumber = 3;
                        }
                        else
                        {
                            // Rule 2 - but not creating internal nodes
                            SuffixTreeNode newLeaf = new SuffixTreeNode(insertingTextLength);
                            SuffixTreeLink newExtendingLink = new SuffixTreeLink(this) { Parent = followingLink.Child, Start = endIndex - 1, Child = newLeaf };
                            newLeaf.ParentLink = newExtendingLink;
                            followingLink.Child.Links.Add(characterToExtend, newExtendingLink);
                            this.lastExtensionSearchNode = followingLink.Child;

                            appliedRuleNumber = 2;
                        }
                    }
                }
                else
                {
                    if (followingLink.EdgeLabel(this.text, linkCursor) == characterToExtend)
                    {
                        appliedRuleNumber = 3;
                    }
                    else
                    {
                        // Rule 2 - creating internal node
                        int originalEnd = followingLink.End;
                        SuffixTreeNode originalChild = followingLink.Child;
                        newlyCreatedInternalNode = followingLink.Child = new SuffixTreeNode(insertingTextLength - 1);
                        newlyCreatedInternalNode.ParentLink = followingLink;
                        followingLink.End = followingLink.Start + linkCursor;
                        SuffixTreeLink brokenLink2 = new SuffixTreeLink(this) { Parent = followingLink.Child, Start = followingLink.Start + linkCursor, Child = originalChild };
                        originalChild.ParentLink = brokenLink2;
                        followingLink.Child.Links.Add(this.text[followingLink.Start + linkCursor], brokenLink2);

                        SuffixTreeNode newLeaf = new SuffixTreeNode(insertingTextLength);
                        SuffixTreeLink newExtendingLink = new SuffixTreeLink(this) { Parent = followingLink.Child, Start = endIndex - 1, Child = newLeaf };
                        newLeaf.ParentLink = newExtendingLink;
                        followingLink.Child.Links.Add(characterToExtend, newExtendingLink);
                        this.lastExtensionSearchNode = newlyCreatedInternalNode;

                        appliedRuleNumber = 2;
                    }
                }
            }
            else
            {
                if (nodeCursor.Links.ContainsKey(characterToExtend))
                {
                    appliedRuleNumber = 3;
                }
                else
                {
                    // Rule 2 - but not creating internal nodes
                    SuffixTreeNode newLeaf = new SuffixTreeNode(insertingTextLength);
                    SuffixTreeLink newExtendingLink = new SuffixTreeLink(this) { Parent = nodeCursor, Start = endIndex - 1, Child = newLeaf };
                    newLeaf.ParentLink = newExtendingLink;
                    nodeCursor.Links.Add(characterToExtend, newExtendingLink);
                    this.lastExtensionSearchNode = nodeCursor;

                    appliedRuleNumber = 2;
                }
            }

            if (this.lastInternalNode != null)
            {
                Debug.Assert(this.lastInternalNodeBirthday == this.extensionCount - 1, "Internal node should get their suffix link in the next extension.");
                if (followingLink != null)
                {
                    Debug.Assert(linkCursor == followingLink.Length(), "We should always end up in another node if we applied rule 2");
                    this.lastInternalNode.SuffixLink = followingLink.Child;
                }
                else
                {
                    this.lastInternalNode.SuffixLink = nodeCursor;
                }

                this.lastInternalNode = null;
            }

            if (newlyCreatedInternalNode != null)
            {
                this.lastInternalNode = newlyCreatedInternalNode;
                this.lastInternalNodeBirthday = this.extensionCount;
            }

            return appliedRuleNumber;
        }

        private void ValidateSuffixLinks(SuffixTreeNode node)
        {
            if (node.ParentLink == null)
            {
                Debug.Assert(node.SuffixLink == null, "Root node should not have a suffix link");
            }
            else if (node.Links.Count == 0)
            {
                Debug.Assert(node.SuffixLink == null, "Leaf node should not have a suffix link");
            }
            else
            {
                Debug.Assert(node.SuffixLink != null, "Internal node should have suffix link");
                Debug.Assert(node.StringDepth == node.SuffixLink.StringDepth + 1, "Should actually point to a suffix");
            }

            foreach (var link in node.Links)
            {
                this.ValidateSuffixLinks(link.Value.Child);
            }
        }

        private class SuffixTreeLink
        {
            private SuffixTree algorithm;
            private int end;

            public SuffixTreeLink(SuffixTree algorithm)
            {
                this.algorithm = algorithm;
            }

            public SuffixTreeNode Parent { get; set; }

            public int Start { get; set; }

            public int End
            {
                get
                {
                    if (this.algorithm == null)
                    {
                        return this.end;
                    }
                    else
                    {
                        return this.algorithm.currentPhaseEnd;
                    }
                }

                set
                {
                    this.end = value;
                    this.algorithm = null;
                }
            }

            public SuffixTreeNode Child { get; set; }

            // 0 based [) index
            public char EdgeLabel(string text, int index)
            {
                return text[this.Start + index];
            }

            public string EdgeLabel(string text)
            {
                return text.Substring(this.Start, this.Length());
            }

            public int Length()
            {
                return this.End - this.Start;
            }
        }

        private class SuffixTreeNode
        {
            private Dictionary<char, SuffixTreeLink> links = new Dictionary<char, SuffixTreeLink>();
            private int stringDepth;

            public SuffixTreeNode(int stringDepth)
            {
                this.stringDepth = stringDepth;
            }

            public Dictionary<char, SuffixTreeLink> Links
            {
                get { return this.links; }
            }

            public SuffixTreeLink ParentLink { get; set; }

            public SuffixTreeNode SuffixLink { get; set; }

            public int StringDepth
            {
                get
                {
                    if (this.links.Count == 0)
                    {
                        return this.ParentLink.Parent.StringDepth + this.ParentLink.Length();
                    }
                    else
                    {
                        return this.stringDepth;
                    }
                }

                set
                {
                    this.stringDepth = value;
                }
            }

            internal void BuildString(string text, StringBuilder sb, int indent)
            {
                sb.Append(new string('+', indent));
                sb.AppendFormat("({0})", this.StringDepth);
                sb.AppendLine();

                foreach (var link in this.Links)
                {
                    sb.Append(new string('-', indent));
                    sb.AppendLine(link.Value.EdgeLabel(text));
                    link.Value.Child.BuildString(text, sb, indent + 2);
                }
            }
        }
    }
}
