#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

static int uTestNum = -1;

using namespace std;

enum Type {  SYMBOL, INT, FLOAT, STRING, ERROR,
                       NIL, T, LEFT_PAREN, RIGHT_PAREN, DOT, QUOTE, ORDER, OURORDER
};

enum Position { LEFT, MIDDLE, RIGHT };

class Tree {
  public:
  int mInum;
  double mFnum;
  string mStr;
  bool mDot;
  bool mPutParen;
  bool mQuoteMember;
  Type mType;
  Position mPosition;
  Tree * mLeft;
  Tree * mRight;
  Tree() {
    mInum = 0, mFnum = 0.0, mStr = "", mDot = false, mPutParen = false, mQuoteMember = false;
    mLeft = NULL, mRight = NULL;
  } // Tree()
};

typedef Tree * TreePtr;

struct Fun {
  string str;
  int parameterNumber;
  vector<string> parameter;
  TreePtr function;
};

struct Eval {
  string str;
  int memorySpaceNumber;
  TreePtr root;
  Type type;
};

struct Para {
  string str;
  TreePtr root;
};

vector<Eval> gDefinition;

vector<Fun> gFunction;

vector<Para> gParameter;

int gLine = 1, gColumn = 0, gMemoryNumber = 0;

int gTempMemoryNumber = 0, gParameterNumber = 0, gArgumentNumber = 0;

string gErrorFunctionName = "", gErrorMessage = "";

TreePtr gErrorPointer = NULL;

bool gChangeLine = true, gTopLevel = true, gExit = false;

bool gAnswerExist = false, gDefineExist = false, gWriteExist = false;

void ReadSExp( TreePtr &root, int &returnVal, bool &error ) ;

void Print( TreePtr head, int &count, bool &enterCheck ) ;

void Error( int returnVal, char character ) ;

void CheckSExp(  string &token, Type &type ) ;

void NumberFix( string &token, Type &type ) ;

void GetToken( string &token, int &returnVal, int &lparenNumber, int  &rparenNumber,
                            bool &error, Type &type ) ;

void BuildTree( TreePtr &root, string &token, int &lparenNumber, int  &rparenNumber, Position &position,
                            Type &type, int &returnVal, bool &error, bool &jump, bool &dot )  ;

void CopyTree( TreePtr &copyRoot, TreePtr root ) ;

void EvalSExp( TreePtr temp, TreePtr &resultSExp, bool &errorCase ) ;

void Cons( TreePtr &resultSExp, bool &errorCase ) ;

void List( TreePtr &resultSExp, bool &errorCase ) ;

void Define( TreePtr &resultSExp, bool &errorCase ) ;

void TravelTreeInLet( TreePtr &root, string name ) ;

void Let( TreePtr &resultSExp, bool &errorCase ) ;

void Lambda( TreePtr &resultSExp, bool &errorCase ) ;

void TravelTree( TreePtr &root ) ;

void OurOrder( TreePtr &resultSExp, bool &errorCase ) ;

void QuoteTree( TreePtr &root ) ;

void Quote( TreePtr &resultSExp, int &count, bool &errorCase ) ;

void Create_Error_Object( TreePtr &resultSExp, bool &errorCase ) ;

void Error_Object( TreePtr &resultSExp, bool &errorCase ) ;

void Read( TreePtr &resultSExp, bool &errorCase ) ;

void Write( TreePtr &resultSExp, bool &errorCase ) ;

void Display_String( TreePtr &resultSExp, bool &errorCase ) ;

void NewLine( TreePtr &resultSExp ) ;

void EvaL( TreePtr &resultSExp, bool &errorCase ) ;

void Set( TreePtr &resultSExp, bool &errorCase ) ;

void Car( TreePtr &resultSExp, bool &errorCase ) ;

void Cdr( TreePtr &resultSExp, bool &errorCase ) ;

void Pair( TreePtr &resultSExp, bool &errorCase ) ;

void Null( TreePtr &resultSExp, bool &errorCase ) ;

void Integer( TreePtr &resultSExp, bool &errorCase ) ;

void Real( TreePtr &resultSExp, bool &errorCase ) ;

void Number( TreePtr &resultSExp, bool &errorCase ) ;

void String( TreePtr &resultSExp, bool &errorCase ) ;

void Boolean( TreePtr &resultSExp, bool &errorCase ) ;

void Symbol( TreePtr &resultSExp, bool &errorCase ) ;

void Result( TreePtr &resultSExp, double &fresult, bool &floatExist, bool &errorCase ) ;

void Plus( TreePtr &resultSExp,
                double &fresult, bool &floatExist,
                bool &firstNumber, bool &errorCase ) ;

void Minus( TreePtr &resultSExp,
                   double &fresult, bool &floatExist,
                   bool &firstNumber, bool &errorCase ) ;

void Multiply( TreePtr &resultSExp,
                      double &fresult, bool &floatExist,
                      bool &firstNumber, bool &errorCase ) ;

void Divide( TreePtr &resultSExp,
                   double &fresult, bool &floatExist,
                   bool &firstNumber, bool &errorCase ) ;

void Not( TreePtr &resultSExp, bool &errorCase ) ;

void And( TreePtr &resultSExp, bool &errorCase ) ;

void Or( TreePtr &resultSExp, bool &errorCase ) ;

void OutCome( TreePtr &resultSExp, bool &outcome, bool &errorCase ) ;

void Greater( TreePtr &resultSExp, double &number,
                  bool &outcome, bool &firstNumber, bool &errorCase ) ;

void GreaterOrEqual( TreePtr &resultSExp, double &number,
                     bool &outcome, bool &firstNumber, bool &errorCase ) ;

void Less( TreePtr &resultSExp, double &number,
                     bool &outcome, bool &firstNumber, bool &errorCase ) ;

void LessOrEqual( TreePtr &resultSExp, double &number,
                     bool &outcome, bool &firstNumber, bool &errorCase ) ;

void Equal( TreePtr &resultSExp, double &number,
                     bool &outcome, bool &firstNumber, bool &errorCase ) ;

void StringAppendResult( TreePtr &resultSExp, string &appendResult, bool &errorCase ) ;

void StringAppend( TreePtr &resultSExp, string &appendResult, bool &errorCase ) ;

void StringCompare( TreePtr &resultSExp, string &currentString,
                    bool &outcome, bool &firstString, bool &errorCase ) ;

void Eqv( TreePtr &resultSExp, bool&errorCase ) ;

void CompareTree( TreePtr root1, TreePtr root2, bool &same ) ;

void EquaL( TreePtr &resultSExp, bool&errorCase ) ;

void Begin( TreePtr &resultSExp, bool&errorCase ) ;

void If( TreePtr &resultSExp, bool&errorCase ) ;

void Cond( TreePtr &resultSExp, bool&errorCase ) ;

void CleanEnvironment( TreePtr &resultSExp, bool&errorCase ) ;

void Exit( TreePtr &resultSExp, bool &errorCase ) ;

void True( TreePtr &resultSExp ) ;

void False( TreePtr &resultSExp ) ;

void CheckBound( TreePtr &resultSExp, bool &errorCase  ) ;

void OurScheme() ;

void ReadSExp( TreePtr &root, int &returnVal, bool &error ) {
  Type type;
  string token = "";
  int lparenNumber = 0, rparenNumber = 0;
  char character = '\0';
  bool jump = false, dot = false;
  Position position = MIDDLE;

  root = NULL, error = false, gLine = 1, gColumn = 0;
  if ( returnVal != EOF && !error ) {
    GetToken( token, returnVal, lparenNumber, rparenNumber, error, type );
    gChangeLine = true;
    BuildTree( root, token, lparenNumber, rparenNumber, position, type, returnVal, error, jump, dot );
  } // if

  if ( returnVal == EOF ) {
    error = true;
    Error( returnVal, character );
  } // if

} // ReadSExp()

void Print( TreePtr head, int &count, bool &enterCheck ) {
  if ( head != NULL ) {
    if ( head->mType == LEFT_PAREN && head->mDot != true ) {
      if ( enterCheck ) {
        for ( int i = 0; i < count ; i++ ) {
          printf( "  " );
        } // for
      } // if

      printf( "%s ", head->mStr.c_str() );
      count++, enterCheck = false;
    } // if

    if ( head->mPosition == RIGHT ) {
      if ( head->mType != NIL  ) {
        if ( enterCheck ) {
          for ( int i = 0; i < count ; i++ ) {
            printf( "  " );
          } // for
        } // if

        printf( ".\n" );
        enterCheck = true;
      } // if
    } // if

    if ( head->mType == INT ) {
      if ( enterCheck ) {
        for ( int i = 0; i < count ; i++ ) {
          printf( "  " );
        } // for
      } // if

      if ( count == 0 && gWriteExist ) printf( "%d", head->mInum );
      else printf( "%d\n", head->mInum );
      enterCheck = true;

      if ( head->mPosition == RIGHT ) {
        count--;
        if ( enterCheck ) {
          for ( int i = 0; i < count ; i++ ) {
            printf( "  " );
          } // for
        } // if

        if ( count == 0 && gWriteExist ) printf( ")" );
        else printf( ")\n" );
        enterCheck = true;
      } // if

    } // if

    if ( head->mType == FLOAT ) {
      if ( enterCheck ) {
        for ( int i = 0; i < count ; i++ ) {
          printf( "  " );
        } // for
      } // if

      if ( count == 0 && gWriteExist ) printf( "%.3f", head->mFnum );
      else printf( "%.3f\n", head->mFnum );
      enterCheck = true;

      if ( head->mPosition == RIGHT ) {
        count--;
        if ( enterCheck ) {
          for ( int i = 0; i < count ; i++ ) {
            printf( "  " );
          } // for
        } // if

        if ( count == 0 && gWriteExist ) printf( ")" );
        else printf( ")\n" );
        enterCheck = true;
      } // if

    } // if

    if ( head->mType != INT && head->mType != FLOAT && head->mType != NIL
         && head->mType != LEFT_PAREN && head->mType != RIGHT_PAREN ) {

      if ( enterCheck ) {
        for ( int i = 0; i < count ; i++ ) {
          printf( "  " );
        } // for
      } // if

      if ( count == 0 && gWriteExist ) printf( "%s", head->mStr.c_str() );
      else printf( "%s\n", head->mStr.c_str() );
      enterCheck = true;

      if ( head->mPosition == RIGHT ) {
        count--;
        if ( enterCheck ) {
          for ( int i = 0; i < count ; i++ ) {
            printf( "  " );
          } // for
        } // if

        if ( count == 0 && gWriteExist ) printf( ")" );
        else printf( ")\n" );
        enterCheck = true;
      } // if

    } // if

    if ( head->mType == NIL && ( head->mPosition == LEFT || head->mPosition == MIDDLE ) ) {
      if ( enterCheck ) {
        for ( int i = 0; i < count ; i++ ) {
          printf( "  " );
        } // for
      } // if

      if ( count == 0 && gWriteExist ) printf( "%s", head->mStr.c_str() );
      else printf( "%s\n", head->mStr.c_str() );
      enterCheck = true;
    } // if

    if ( head->mType == NIL && head->mPosition == RIGHT ) {
      count--;
      if ( enterCheck ) {
        for ( int i = 0; i < count ; i++ ) {
          printf( "  " );
        } // for
      } // if

      if ( count == 0 && gWriteExist ) printf( ")" );
      else printf( ")\n" );
      enterCheck = true;
    } // if

    Print( head->mLeft, count, enterCheck );
    Print( head->mRight, count, enterCheck );
  } // if

} // Print()

void Error( int returnVal, char character ) {
  char buffer[ 200 ];
  gErrorMessage.clear();

  if ( returnVal == EOF ) {
    gErrorMessage.append( "ERROR : END-OF-FILE encountered when there should be more input" );
    return;
  } // if

  else {
    if ( character == '\n' ) {
      gErrorMessage.append( "ERROR (unexpected character) : " );
      sprintf( buffer, "line %d column %d LINE-ENTER encountered", gLine, gColumn );
      gErrorMessage.append( buffer );
    } // if

    else {
      gErrorMessage.append( "ERROR (unexpected character) : " );
      sprintf( buffer, "line %d column %d character '%c'", gLine, gColumn, character  );
      gErrorMessage.append( buffer );
    } // else

    for ( ; character != '\n' && returnVal != EOF  ; ) {
      returnVal = scanf( "%c", &character );
    } // for

  } // else

} // Error()

void CheckSExp(  string &token, Type &type ) {

  for ( int i = 0  ; i < gDefinition.size() ; i++ ) {
    if ( gDefinition[i].str.compare( token ) == 0 &&  gDefinition[i].type == ERROR  ) {
      type = ERROR;
      return;
    } // if
  } // for

  for ( int i = 0  ; i < gFunction.size() ; i++ ) {
    if ( gFunction[i].str.compare( token ) == 0 ) {
      type = OURORDER;
      return;
    } // if
  } // for

  if ( token.compare( "t" ) == 0 || token.compare( "#t" ) == 0 ) token = "#t", type = T;

  else if ( token.compare( "#f" ) == 0
            || token.compare( "nil" ) == 0  ) token = "nil", type = NIL;

  else if ( token.compare( "quote" ) == 0 ) type = QUOTE;

  else if ( token.compare( "+." ) == 0 || token.compare( "-." ) == 0 ) type = SYMBOL;

  else if ( token.compare( "clean-environment" ) == 0 || token.compare( "exit" ) == 0
            || token.compare( "cons" ) == 0 || token.compare( "list" ) == 0
            || token.compare( "define" ) == 0 || token.compare( "car" ) == 0
            || token.compare( "cdr" ) == 0 || token.compare( "pair?" ) == 0
            || token.compare( "null?" ) == 0 || token.compare( "integer?" ) == 0
            || token.compare( "real?" ) == 0 || token.compare( "number?" ) == 0
            || token.compare( "string?" ) == 0 || token.compare( "boolean?" ) == 0
            || token.compare( "symbol?" ) == 0 || token.compare( "+" ) == 0
            || token.compare( "-" ) == 0 || token.compare( "*" ) == 0
            || token.compare( "/" ) == 0 || token.compare( "not" ) == 0
            || token.compare( "and" ) == 0 || token.compare( "or" ) == 0
            || token.compare( ">" ) == 0 || token.compare( ">=" ) == 0
            || token.compare( "<" ) == 0 || token.compare( "<=" ) == 0
            || token.compare( "=" ) == 0 || token.compare( "string-append" ) == 0
            || token.compare( "string>?" ) == 0 || token.compare( "eqv?" ) == 0
            || token.compare( "equal?" ) == 0 || token.compare( "begin" ) == 0
            || token.compare( "if" ) == 0 || token.compare( "cond" ) == 0
            || token.compare( "let" ) == 0 || token.compare( "lambda" ) == 0
            || token.compare( "create-error-object" ) == 0 || token.compare( "error-object?" ) == 0
            || token.compare( "read" ) == 0 || token.compare( "write" ) == 0
            || token.compare( "display-string" ) == 0 || token.compare( "newline" ) == 0
            || token.compare( "eval" ) == 0  || token.compare( "set!" ) == 0   ) type = ORDER;

  else {
    NumberFix( token, type );
  } // else

} // CheckSExp()

void NumberFix( string &token, Type &type ) {
  int plusNumber = 0, minusNumber = 0, dotNumber = 0;
  string oneZero = "0", doubleZero = "00", tripleZero = "000";

  if ( token.size() == 1 && !isdigit( token[0] ) ) return;

  for ( int i = 0; i < token.size() ; i++ ) {
    if ( isprint( token[i] ) && !isdigit( token[i] )
         && token[i] != '+' && token[i] != '-' && token[i] != '.'  ) return;
  } // for

  for ( int i = 0; i < token.size() ; i++ ) {
    if ( token[i] == '+' )  plusNumber++;

    if ( token[i] == '-' )  minusNumber++;

    if ( token[i] == '.' )  dotNumber++;

    if (  plusNumber > 1 || minusNumber > 1 || dotNumber > 1 ) return;

    else if ( plusNumber + minusNumber > 1 ) return;
  } // for

  for ( int i = 0; i < token.size() ; i++ ) {
    if ( token[i] == '+' && i != 0 ) return;

    if ( token[i] == '-' && i != 0 ) return;
  } // for

  if ( dotNumber == 1 ) type = FLOAT;

  if ( dotNumber == 0 ) type = INT;

  if ( token.size() > 1 ) {
    if ( token[token.size()-1] == '.' ) {
      type = FLOAT;
      token = token + tripleZero;
    } // if

    if ( token[0] == '+' ) {
      token.erase( token.begin() );
    } // if

    if ( token[0] == '.' ) {
      type = FLOAT;
      token = oneZero + token;
    } // if
  } // if

  if ( token.size() >= 2 ) {
    if ( token[token.size()-2] == '.' ) {
      token = token + doubleZero;
      type = FLOAT;
    } // if
  } // if

  if ( token.size() >= 3 ) {
    if ( token[token.size()-3] == '.' ) {
      token = token + oneZero;
      type = FLOAT;
    } // if

    if ( token[0] == '-' && token[1] == '.' ) {
      token.erase( token.begin() );
      token = "-0" + token;
      type = FLOAT;
    } // if
  } // if

} // NumberFix()

void GetToken( string &token, int &returnVal, int &lparenNumber, int  &rparenNumber,
                            bool &error, Type &type ) {
  char character = '\0';
  bool jump = false;
  token.clear();

  returnVal = scanf( "%c", &character ), gColumn++;

  while ( returnVal != EOF ) {
    if ( character == '"' ) {
      token.push_back( character );
      returnVal = scanf( "%c", &character ), gColumn++;

      while ( character != '"' && character != '\n' && returnVal != EOF ) {
        token.push_back( character );
        returnVal = scanf( "%c", &character ), gColumn++;
      } // while

      if ( returnVal == EOF ) return;

      if ( character == '\n' ) {
        Error( returnVal, character );
        token.clear(), error = true;
        return;
      } // if

      else {
        type = STRING;
        token.push_back( character );
      } // else

      return;
    } // if

    else if ( character == ';' ) {
      while ( character != '\n' && returnVal != EOF ) {
        returnVal = scanf( "%c", &character ), gColumn++;
      } // while

      if ( returnVal == EOF ) return;

      if ( character == '\n' && gChangeLine ) gLine++, gColumn = 0;
      gChangeLine = true;
    } // else if

    else if ( character == ' ' ) ;

    else if ( character == '\n' ) {
      if ( gChangeLine ) gLine++, gColumn = 0;
      else gColumn = 0;
      gChangeLine = true;
    } // else if

    else if ( character == '(' ) {
      lparenNumber++;
      type = LEFT_PAREN;
      token.push_back( character );
      return;
    } // else if

    else if ( character == ')' ) {
      if ( lparenNumber == 0 ) {
        Error( returnVal, character );
        token.clear(), error = true;
        return;
      } // if

      else {
        rparenNumber++;
        type = RIGHT_PAREN;
        token.push_back( character );
      } // else

      return;
    } // else if

    else if ( character == '\'' ) {
      type = QUOTE;
      token.push_back( character );
      return;
    } // else if

    else if ( character == '.'
              && ( cin.peek() == ' ' || cin.peek() == '\n'
                   || cin.peek() == '\'' || cin.peek() == '"'
                   || cin.peek() == '(' || cin.peek() == ')' ) ) {
      type = DOT;
      token.push_back( character );
      return;
    } // else if

    else {
      jump = false;
      while ( !jump ) {
        token.push_back( character );

        if ( cin.peek() == '(' || cin.peek() == ')'  || cin.peek() == '\n'
             || cin.peek() == ' ' || cin.peek() == '\'' || cin.peek() == '"'
             || cin.peek() == EOF ) {
          jump = true;
        } // if

        else {
          returnVal = scanf( "%c", &character ), gColumn++;
        } // else
      } // while

      type = SYMBOL;
      CheckSExp( token, type );
      return;
    } // else

    returnVal = scanf( "%c", &character ), gColumn++;

  } // while

} // GetToken()

void BuildTree( TreePtr &root, string &token, int &lparenNumber, int  &rparenNumber, Position &position,
                            Type &type, int &returnVal, bool &error, bool &jump, bool &dot ) {
  char ch = '\0';

  while ( !jump && !error && returnVal != EOF  ) {

    if ( token.empty() ) {
      if ( lparenNumber != rparenNumber || lparenNumber == 0  ) {
        GetToken( token, returnVal, lparenNumber, rparenNumber, error, type );
        if ( returnVal == EOF ) return;
      } // if

      else return;

    } // if

    if ( token.compare( "(" ) == 0 ) {
      if ( root == NULL ) {
        root = new Tree;
        root->mStr = token, root->mType = LEFT_PAREN, root->mPosition = MIDDLE, root->mDot = dot;

        if ( lparenNumber != rparenNumber || lparenNumber == 0  ) {
          GetToken( token, returnVal, lparenNumber, rparenNumber, error, type );
          if ( returnVal == EOF ) return;
        } // if

        else return;

        if ( token.compare( ")" ) == 0 ) {
          root->mStr = "nil", root->mType = NIL, root->mPosition = position, root->mDot = dot;
          token.clear(), dot = false;
          return;
        } // if

        else {
          position = LEFT, dot = false;
          BuildTree( root->mLeft, token, lparenNumber, rparenNumber,
                              position, type, returnVal, error, jump, dot );
        } // else

      } // if

      else if ( root->mLeft == NULL ) {
        position = LEFT;
        BuildTree( root->mLeft, token, lparenNumber, rparenNumber,
                            position, type, returnVal, error, jump, dot );
      } // else if

      else if ( root->mRight == NULL ) {
        root->mRight = new Tree;
        root->mRight->mStr = "(", root->mRight->mType = LEFT_PAREN;
        root->mRight->mPosition = MIDDLE, root->mRight->mDot = true, root->mRight->mPutParen = true;
        position = RIGHT;
        BuildTree( root->mRight, token, lparenNumber, rparenNumber,
                            position, type, returnVal, error, jump, dot );
      } // else if

      else {
        error = true;
        ch = token[0];
        Error( returnVal, ch );
      } // else

    } // if

    else if ( token.compare( ")" ) == 0 ) {
      if ( root == NULL ) {
        error = true;
        ch = token[0];
        Error( returnVal, ch );
      } // if

      else if ( root->mLeft != NULL && root->mRight == NULL ) {
        root->mRight = new Tree;
        root->mRight->mStr = "nil", root->mRight->mType = NIL;
        root->mRight->mPosition = RIGHT;

        if ( root->mType == LEFT_PAREN && root->mPutParen == true ) {
          return;
        } // if

        else if ( root->mType == LEFT_PAREN  ) {
          token.clear(), dot = false;
        } // else if
      } // else if

      else if ( root->mLeft != NULL && root->mRight != NULL
                && root->mPutParen == true ) {
        return;
      } // else if

      else {
        token.clear(), dot = false;
      } // else

      return;

    } // else if

    else if ( token.compare( "." ) == 0 ) {

      if ( root == NULL && ( position == LEFT || position == MIDDLE ) ) {
        error = true;
        returnVal = scanf( "%c", &ch ), gColumn++;
        Error( returnVal, ch );
      } // if

      else if ( root->mRight != NULL  ) {
        error = true;
        ch = token[0];
        Error( returnVal, ch );
      } // else if

      else if ( root->mRight == NULL ) {
        token.clear(), dot = true, position = RIGHT;
        BuildTree( root->mRight, token, lparenNumber, rparenNumber,
                             position, type, returnVal, error, jump, dot );
      } // else if

    } // else if

    else if ( token.compare( "'" ) == 0 ) {
      if ( root == NULL ) {
        root = new Tree;
        root->mStr = "(", root->mType = LEFT_PAREN, root->mPosition = MIDDLE, root->mDot = false;

        root->mLeft = new Tree;
        root->mLeft->mStr = "quote", root->mLeft->mType = QUOTE;
        root->mLeft->mPosition = LEFT;

        root->mRight = new Tree;
        root->mRight->mStr = "(", root->mRight->mType = LEFT_PAREN;
        root->mRight->mPosition = MIDDLE, root->mRight->mDot = true, root->mRight->mPutParen = true;

        root->mRight->mRight = new Tree;
        root->mRight->mRight->mStr = "nil", root->mRight->mRight->mType = NIL;
        root->mRight->mRight->mPosition = RIGHT;

        token.clear(), dot = false, position = LEFT;
        BuildTree( root->mRight->mLeft, token, lparenNumber, rparenNumber,
                            position, type, returnVal, error, jump, dot );
        return;

      } // if

      else if ( root->mLeft == NULL ) {
        BuildTree( root->mLeft, token, lparenNumber, rparenNumber,
                            position, type, returnVal, error, jump, dot );
      } // else if

      else if ( root->mRight == NULL ) {
        root->mRight = new Tree;
        root->mRight->mStr = "(", root->mRight->mType = LEFT_PAREN;
        root->mRight->mPosition = MIDDLE, root->mRight->mDot = true, root->mRight->mPutParen = true;
        position = RIGHT;
        BuildTree( root->mRight, token, lparenNumber, rparenNumber,
                            position, type, returnVal, error, jump, dot );
      } // else if

    } // else if

    else if ( !token.empty() ) {
      if ( root != NULL ) {

        if ( root->mLeft == NULL ) {
          position = LEFT, dot = false;
          BuildTree( root->mLeft, token, lparenNumber, rparenNumber,
                              position, type, returnVal, error, jump, dot );
        } // if

        else if ( root->mRight == NULL ) {
          root->mRight = new Tree;
          root->mRight->mStr = "(", root->mRight->mType = LEFT_PAREN;
          root->mRight->mPosition = MIDDLE, root->mRight->mDot = true, root->mRight->mPutParen = true;
          position = RIGHT;
          BuildTree( root->mRight, token, lparenNumber, rparenNumber,
                               position, type, returnVal, error, jump, dot );
        } // else if

        else {
          error = true;
          ch = token[0];
          if ( token.size() > 1 ) gColumn = gColumn - ( token.size() - 1 );
          Error( returnVal, ch );
        } // else

      } // if

      else {
        root = new Tree;
        if ( type == INT ) {
          root->mInum = atoi( token.c_str() ), root->mType = INT, root->mPosition = position;
        } // if

        else if ( type ==  FLOAT ) {
          root->mFnum = atof( token.c_str() ), root->mType = FLOAT, root->mPosition = position;
        } // else if

        else {
          root->mStr = token, root->mType = type, root->mPosition = position;
        } // else

        token.clear(), dot = false;
        return;
      } // else

    } // else if

  } // while

  if ( error ) root = NULL;

} // BuildTree()

void CopyTree( TreePtr &copyRoot, TreePtr root ) {
  if ( root != NULL ) {
    copyRoot = new Tree;
    copyRoot->mInum = root->mInum;
    copyRoot->mFnum = root->mFnum;
    copyRoot->mStr = root->mStr;
    copyRoot->mDot = root->mDot;
    copyRoot->mQuoteMember = root->mQuoteMember;
    copyRoot->mPosition = root->mPosition;
    copyRoot->mType = root->mType;

    CopyTree( copyRoot->mLeft, root->mLeft );
    CopyTree( copyRoot->mRight, root->mRight );
  } // if
} // CopyTree()

void EvalSExp( TreePtr temp, TreePtr &resultSExp, bool &errorCase ) {
  int count = 0;
  char buffer[ 200 ];
  bool firstNumber = true, firstString = true ;
  bool outcome = true, floatExist = false;
  double fresult = 0.0, number = 0.0;
  Para store;
  string appendResult = "", currentString = "";
  resultSExp = temp;

  if ( resultSExp != NULL && !errorCase ) {
    if (  resultSExp->mStr.compare( "(" ) == 0 ) {

      if ( resultSExp->mLeft->mStr.compare( "(" ) == 0 ) {
        gTopLevel = false, gArgumentNumber = 0, gParameter.clear() ;

        for ( TreePtr walk = resultSExp->mRight ; walk->mStr.compare( "nil" ) != 0
              && !errorCase ; walk = walk->mRight ) {
          if ( !gDefineExist ) EvalSExp( walk->mLeft, walk->mLeft, errorCase );
          CopyTree( store.root, walk->mLeft );
          gParameter.push_back( store );
          gArgumentNumber++;
        } // for

        EvalSExp( resultSExp->mLeft, resultSExp->mLeft, errorCase );
        if ( errorCase || gDefineExist ) return;
      } // if

      if ( resultSExp->mLeft->mStr.compare( "cons" ) == 0 ) {
        Cons( resultSExp, errorCase );
      } // if

      else if ( resultSExp->mLeft->mStr.compare( "list" ) == 0 ) {
        List( resultSExp, errorCase );
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( "quote" ) == 0 ) {
        Quote( resultSExp, count, errorCase );
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( "create-error-object" ) == 0 ) {
        Create_Error_Object( resultSExp, errorCase );
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( "error-object?" ) == 0 ) {
        Error_Object( resultSExp, errorCase );
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( "read" ) == 0 ) {
        Read( resultSExp, errorCase );
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( "write" ) == 0 ) {
        Write( resultSExp, errorCase );
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( "display-string" ) == 0 ) {
        Display_String( resultSExp, errorCase );
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( "newline" ) == 0 ) {
        NewLine( resultSExp );
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( "eval" ) == 0 ) {
        EvaL( resultSExp, errorCase );
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( "set!" ) == 0 ) {
        Set( resultSExp, errorCase );
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( "define" ) == 0 ) {
        Define( resultSExp, errorCase );
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( "let" ) == 0 ) {
        Let( resultSExp, errorCase );
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( "lambda" ) == 0 ) {
        Lambda( resultSExp, errorCase );
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( "car" ) == 0 ) {
        Car( resultSExp, errorCase );
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( "cdr" ) == 0 ) {
        Cdr( resultSExp, errorCase );
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( "pair?" ) == 0 ) {
        Pair( resultSExp, errorCase );
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( "null?" ) == 0 ) {
        Null( resultSExp, errorCase );
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( "integer?" ) == 0 ) {
        Integer( resultSExp, errorCase );
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( "real?" ) == 0 ) {
        Real( resultSExp, errorCase );
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( "number?" ) == 0 ) {
        Number( resultSExp, errorCase );
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( "string?" ) == 0 ) {
        String( resultSExp, errorCase );
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( "boolean?" ) == 0 ) {
        Boolean( resultSExp, errorCase );
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( "symbol?" ) == 0 ) {
        Symbol( resultSExp, errorCase );
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( "+" ) == 0 ) {
        Plus( resultSExp, fresult,
                  floatExist, firstNumber, errorCase );

        Result( resultSExp, fresult, floatExist, errorCase ) ;
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( "-" ) == 0 ) {
        Minus( resultSExp, fresult,
                  floatExist, firstNumber, errorCase );

        Result( resultSExp, fresult, floatExist, errorCase ) ;
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( "*" ) == 0 ) {
        Multiply( resultSExp, fresult,
                  floatExist, firstNumber, errorCase );

        Result( resultSExp, fresult, floatExist, errorCase ) ;
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( "/" ) == 0 ) {
        Divide( resultSExp, fresult,
                  floatExist, firstNumber, errorCase );

        Result( resultSExp, fresult, floatExist, errorCase ) ;
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( "not" ) == 0 ) {
        Not( resultSExp, errorCase );
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( "and" ) == 0 ) {
        And( resultSExp, errorCase );
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( "or" ) == 0 ) {
        Or( resultSExp, errorCase );
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( ">" ) == 0 ) {
        Greater( resultSExp, number,
                  outcome, firstNumber, errorCase );

        OutCome( resultSExp, outcome, errorCase );
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( ">=" ) == 0 ) {
        GreaterOrEqual( resultSExp, number,
                        outcome, firstNumber, errorCase );

        OutCome( resultSExp, outcome, errorCase );
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( "<" ) == 0 ) {
        Less( resultSExp, number,
                  outcome, firstNumber, errorCase );

        OutCome( resultSExp, outcome, errorCase );
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( "<=" ) == 0 ) {
        LessOrEqual( resultSExp, number,
                     outcome, firstNumber, errorCase );

        OutCome( resultSExp, outcome, errorCase );
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( "=" ) == 0 ) {
        Equal( resultSExp, number,
                  outcome, firstNumber, errorCase );

        OutCome( resultSExp, outcome, errorCase );
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( "string-append" ) == 0 ) {
        StringAppend( resultSExp, appendResult, errorCase );
        StringAppendResult( resultSExp, appendResult, errorCase ) ;
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( "string>?" ) == 0 ) {
        StringCompare( resultSExp, currentString,
                       outcome, firstString, errorCase );

        OutCome( resultSExp, outcome, errorCase );
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( "eqv?" ) == 0 ) {
        Eqv( resultSExp, errorCase );
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( "equal?" ) == 0 ) {
        EquaL( resultSExp, errorCase );
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( "begin" ) == 0 ) {
        Begin( resultSExp, errorCase );
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( "if" ) == 0 ) {
        If( resultSExp, errorCase );
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( "cond" ) == 0 ) {
        Cond( resultSExp, errorCase );
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( "clean-environment" ) == 0 ) {
        CleanEnvironment( resultSExp, errorCase );
      } // else if

      else if ( resultSExp->mLeft->mStr.compare( "exit" ) == 0 ) {
        Exit( resultSExp, errorCase );
      } // else if

      else if ( resultSExp->mLeft->mType == OURORDER ) {
        OurOrder( resultSExp, errorCase );
      } // else if

      else {
        gErrorMessage.clear();

        if ( gAnswerExist ) {
          gAnswerExist = false;
          resultSExp = resultSExp->mLeft;
          return;
        } // if

        if ( resultSExp->mLeft->mType == INT && resultSExp->mDot == false ) {
          errorCase = true;
          sprintf( buffer, "ERROR (attempt to apply non-function) : %d", resultSExp->mLeft->mInum );
          gErrorMessage.append( buffer ), resultSExp = NULL;
        } // if

        else if ( resultSExp->mLeft->mType == FLOAT && resultSExp->mDot == false ) {
          errorCase = true;
          sprintf( buffer, "ERROR (attempt to apply non-function) : %.3f", resultSExp->mLeft->mFnum );
          gErrorMessage.append( buffer );
          resultSExp = NULL;
        } // else if

        else if (  resultSExp->mLeft->mType == STRING && resultSExp->mDot == false ) {
          errorCase = true;
          sprintf( buffer, "ERROR (attempt to apply non-function) : %s", resultSExp->mLeft->mStr.c_str() );
          gErrorMessage.append( buffer );
          resultSExp = NULL;
        } // else if

        else if ( resultSExp->mLeft->mType == SYMBOL ) {
          CheckBound( resultSExp->mLeft, errorCase );

          if ( !errorCase ) {
            gErrorMessage.clear();

            if ( resultSExp->mLeft->mType == INT && resultSExp->mDot == false ) {
              errorCase = true;
              sprintf( buffer, "ERROR (attempt to apply non-function) : %d", resultSExp->mLeft->mInum );
              gErrorMessage.append( buffer ), resultSExp = NULL;
            } // if

            else if ( resultSExp->mLeft->mType == FLOAT && resultSExp->mDot == false ) {
              errorCase = true;
              sprintf( buffer, "ERROR (attempt to apply non-function) : %.3f", resultSExp->mLeft->mFnum );
              gErrorMessage.append( buffer ), resultSExp = NULL;
            } // else if

            else if ( resultSExp->mLeft->mType == STRING && resultSExp->mDot == false ) {
              errorCase = true;
              sprintf( buffer, "ERROR (attempt to apply non-function) : %s",
                           resultSExp->mLeft->mStr.c_str() );
              gErrorMessage.append( buffer ), resultSExp = NULL;
            } // else if

            else if ( resultSExp->mLeft->mType == ORDER && resultSExp->mDot == false ) {
              errorCase = true;
              sprintf( buffer, "ERROR (attempt to apply non-function) : %s",
                            resultSExp->mLeft->mStr.c_str() );
              gErrorMessage.append( buffer ), resultSExp = NULL;
            } // else if
          } // if

        } // else if

        else {
          errorCase = true, gErrorPointer = NULL, gErrorMessage.clear();
          sprintf( buffer, "ERROR (attempt to apply non-function) : " );
          CopyTree( gErrorPointer, resultSExp->mLeft );
          return;
        } // else

        return;
      } // else

    } // if

    else {

      if ( resultSExp->mType == SYMBOL || resultSExp->mType == ERROR  ) {
        CheckBound( resultSExp, errorCase );
        return;
      } // if

      else if ( resultSExp->mType == ORDER || resultSExp->mType == OURORDER  ) {
        resultSExp->mStr = "#function", resultSExp->mType = ORDER ;
        resultSExp->mLeft = NULL, resultSExp->mRight = NULL;
        return;
      } // else if

    } // else

  } // if

} // EvalSExp()

void Cons( TreePtr &resultSExp, bool &errorCase ) {
  char buffer[ 200 ];
  Position position;
  gErrorMessage.clear();

  if ( resultSExp->mRight->mStr.compare( "nil" ) == 0
       || resultSExp->mRight->mRight->mStr.compare( "nil" ) == 0
       || resultSExp->mRight->mRight->mRight->mStr.compare( "nil" ) != 0 ) {
    errorCase = true;
    if ( gErrorFunctionName.compare( "" ) != 0 ) {
      sprintf( buffer, "ERROR (incorrect number of arguments) : %s", gErrorFunctionName.c_str()  );
      gErrorMessage.append( buffer );
    } // if

    else gErrorMessage.append( "ERROR (incorrect number of arguments) : cons" );
    return;
  } // if

  else {
    gTopLevel = false;
    position = resultSExp->mPosition;
    resultSExp = resultSExp->mRight;
    EvalSExp( resultSExp->mLeft, resultSExp->mLeft, errorCase );
    resultSExp->mRight = resultSExp->mRight->mLeft;
    EvalSExp( resultSExp->mRight, resultSExp->mRight, errorCase );

    if ( !errorCase ) {
      resultSExp->mDot = false;
      if ( resultSExp->mRight->mStr.compare( "(" ) == 0 ) resultSExp->mRight->mDot = true;
      else resultSExp->mRight->mPosition = RIGHT;
    } // if
  } // else

} // Cons()

void List( TreePtr &resultSExp, bool &errorCase ) {
  int count = 0;
  char buffer[ 200 ];
  bool enterCheck = false;
  gErrorMessage.clear();

  if ( resultSExp->mRight->mStr.compare( "nil" ) == 0 ) {
    errorCase = true;
    if ( gErrorFunctionName.compare( "" ) != 0 ) {
      sprintf( buffer, "ERROR (incorrect number of arguments) : %s", gErrorFunctionName.c_str()  );
      gErrorMessage.append( buffer );
    } // if

    else gErrorMessage.append( "ERROR (incorrect number of arguments) : list" );
    return;
  } // if

  else {
    gTopLevel = false;
    for ( TreePtr walk = resultSExp->mRight ; walk->mStr.compare( "nil" ) != 0
          && !errorCase ; walk = walk->mRight ) {
      if ( walk->mStr.compare( "(" ) != 0 ) {
        errorCase = true;
        printf( "> ERROR (non-list) : " );
        Print( resultSExp, count, enterCheck );
        printf( "\n" );
        return;
      } // if

      else EvalSExp( walk->mLeft, walk->mLeft, errorCase );
    } // for

    resultSExp = resultSExp->mRight;
    resultSExp->mDot = false;
  } // else

} // List()

void Define( TreePtr &resultSExp, bool &errorCase ) {
  Eval temp;
  Fun tempFunction;
  TreePtr walk = NULL;

  if ( resultSExp->mRight->mStr.compare( "nil" ) == 0
       || resultSExp->mRight->mRight->mStr.compare( "nil" ) == 0
       || resultSExp->mRight->mRight->mRight->mStr.compare( "nil" ) != 0 ) {
    errorCase = true;
    gErrorMessage.append( "ERROR (define format)" );
    return;
  } // if

  else {
    if ( gTopLevel ) {
      gTopLevel = false;

      walk = resultSExp->mRight;

      if ( walk->mLeft->mType == SYMBOL || walk->mLeft->mType == OURORDER
           || walk->mLeft->mType == ERROR ) {

        if ( walk->mRight->mLeft->mStr.compare( "(" ) == 0
             && walk->mRight->mLeft->mLeft->mStr.compare( "lambda" ) == 0  ) {
          gDefineExist = true;
          EvalSExp( walk->mRight->mLeft, walk->mRight->mLeft, errorCase );
          tempFunction.str = walk->mLeft->mStr;
          tempFunction.parameterNumber = gParameterNumber;
          CopyTree( tempFunction.function, walk->mRight->mLeft );

          for ( int i = 0  ; i < gParameterNumber ; i++ ) {
            tempFunction.parameter.push_back( gParameter[i].str );
          } // for

          for ( int j = 0  ; j < gFunction.size() ; j++ ) {
            if ( gFunction[j].str.compare( tempFunction.str ) == 0 ) {
              gFunction.erase( gFunction.begin()+j );
            } // if
          } // for

          for ( int j = 0  ; j < gDefinition.size() ; j++ ) {
            if ( gDefinition[j].str.compare( tempFunction.str ) == 0 ) {
              gDefinition.erase( gDefinition.begin()+j );
            } // if
          } // for

          gFunction.push_back( tempFunction );
          printf( "%s defined\n", tempFunction.str.c_str() );
          resultSExp = NULL;
        } // if

        else if (  walk->mRight->mLeft->mType == ORDER ) {
          tempFunction.str = walk->mLeft->mStr;
          CopyTree( tempFunction.function, walk->mRight->mLeft );
          tempFunction.parameterNumber = 0;

          for ( int j = 0  ; j < gFunction.size() ; j++ ) {
            if ( gFunction[j].str.compare( tempFunction.str ) == 0 ) {
              gFunction.erase( gFunction.begin()+j );
            } // if
          } // for

          for ( int j = 0  ; j < gDefinition.size() ; j++ ) {
            if ( gDefinition[j].str.compare( tempFunction.str ) == 0 ) {
              gDefinition.erase( gDefinition.begin()+j );
            } // if
          } // for

          gFunction.push_back( tempFunction );
          printf( "%s defined\n", tempFunction.str.c_str() );
          resultSExp = NULL;
        } // else if

        else if (  walk->mRight->mLeft->mType == OURORDER ) {
          for ( int i = 0 ; i < gFunction.size() ; i++ ) {
            if ( gFunction[i].str.compare(  walk->mRight->mLeft->mStr ) == 0 ) {
              tempFunction = gFunction[i];
            } // if
          } // for

          for ( int j = 0  ; j < gFunction.size() ; j++ ) {
            if ( gFunction[j].str.compare( walk->mLeft->mStr ) == 0 ) {
              gFunction.erase( gFunction.begin()+j );
            } // if
          } // for

          for ( int j = 0  ; j < gDefinition.size() ; j++ ) {
            if ( gDefinition[j].str.compare( walk->mLeft->mStr ) == 0 ) {
              gDefinition.erase( gDefinition.begin()+j );
            } // if
          } // for

          tempFunction.str = walk->mLeft->mStr;
          gFunction.push_back( tempFunction );
          printf( "%s defined\n", tempFunction.str.c_str() );
          resultSExp = NULL;
        } // else if

        else {
          if ( walk->mLeft->mType == ERROR ) walk->mLeft->mType = SYMBOL;
          temp.str = walk->mLeft->mStr;
          temp.type = walk->mLeft->mType;
          temp.memorySpaceNumber = gMemoryNumber;

          walk = walk->mRight;
          temp.root = walk->mLeft;

          if ( temp.root->mType == SYMBOL ) {
            for ( int i = 0; i < gDefinition.size() ; i++ ) {
              if ( gDefinition[i].str.compare( temp.root->mStr ) == 0 ) {
                temp.root = gDefinition[i].root;
                temp.memorySpaceNumber = gDefinition[i].memorySpaceNumber;
              } // if
            } // for
          } // if

          else {
            EvalSExp( temp.root, temp.root, errorCase );
          } // else

          if ( !errorCase ) {
            for ( int j = 0; j < gDefinition.size() ; j++ ) {
              if ( gDefinition[j].str.compare( temp.str ) == 0 ) {
                gDefinition.erase( gDefinition.begin()+j );
              } // if
            } // for

            for ( int j = 0  ; j < gFunction.size() ; j++ ) {
              if ( gFunction[j].str.compare( temp.str ) == 0 ) {
                gFunction.erase( gFunction.begin()+j );
              } // if
            } // for

            if ( temp.root->mType == ERROR ) temp.type = ERROR;
            gDefinition.push_back( temp );
            printf( "%s defined\n", temp.str.c_str() );
            resultSExp = NULL, gMemoryNumber++;
          } // if
        } // else

      } // if

      else if ( walk->mLeft->mType == LEFT_PAREN ) {

        for ( TreePtr walker2 = walk->mLeft
              ; walker2->mStr.compare( "nil" ) != 0 ; walker2 = walker2->mRight ) {
          if ( walker2->mDot == false ) {
            tempFunction.str = walker2->mLeft->mStr;
            tempFunction.parameterNumber = 0;
          } // if

          else {
            tempFunction.parameter.push_back( walker2->mLeft->mStr );
            tempFunction.parameterNumber++;
          } // else

        } // for

        if ( walk->mRight->mLeft->mStr.compare( "(" ) == 0
             && ( walk->mRight->mLeft->mLeft->mStr.compare( "lambda" ) == 0
                  || ( walk->mRight->mLeft->mLeft->mStr.compare( "(" ) == 0
                       && walk->mRight->mLeft->mLeft->mLeft->mStr.compare( "lambda" ) == 0  )  ) ) {
          gDefineExist = true;
          EvalSExp( walk->mRight->mLeft, walk->mRight->mLeft, errorCase );
          CopyTree( tempFunction.function, walk->mRight->mLeft->mLeft );
        } // if

        else {
          CopyTree( tempFunction.function, walk->mRight->mLeft );
        } // else

        for ( int j = 0; j < gDefinition.size() ; j++ ) {
          if ( gDefinition[j].str.compare( tempFunction.str ) == 0 ) {
            gDefinition.erase( gDefinition.begin()+j );
          } // if
        } // for

        for ( int j = 0  ; j < gFunction.size() ; j++ ) {
          if ( gFunction[j].str.compare( tempFunction.str ) == 0 ) {
            gFunction.erase( gFunction.begin()+j );
          } // if
        } // for

        gFunction.push_back( tempFunction );
        printf( "%s defined\n", tempFunction.str.c_str() );
        resultSExp = NULL;

      } // else if

      else {
        errorCase = true;
        gErrorMessage.append( "ERROR (define format)" );
      } // else
    } // if

    else {
      errorCase = true;
      gErrorMessage.append( "ERROR (define format)" );
    } // else
  } // else

} // Define()

void TravelTreeInLet( TreePtr &root, string name ) {
  if ( root != NULL ) {
    if ( root->mStr.compare( name ) == 0 ) {
      root->mType = SYMBOL;
    } // if

    TravelTreeInLet( root->mLeft, name );
    TravelTreeInLet( root->mRight, name );
  } // if

} // TravelTreeInLet()

void Let( TreePtr &resultSExp, bool &errorCase ) {
  Eval temp;
  int localVariableNumber = 0;
  char buffer[ 200 ];
  bool exist = false;
  TreePtr answer = NULL, walk = NULL;
  TreePtr walk2 = NULL, walk3 = NULL, walk4 = NULL;
  gErrorMessage.clear();

  if ( resultSExp->mRight->mStr.compare( "nil" ) == 0
       || resultSExp->mRight->mRight->mStr.compare( "nil" ) == 0 ) {
    errorCase = true;
    gErrorMessage.append( "ERROR (LET format)" );
    return;
  } // if

  else {
    walk = resultSExp->mRight;
    walk2 = walk->mLeft;

    if ( walk2->mStr.compare( "nil" ) != 0
         && walk2->mStr.compare( "(" ) != 0 ) {
      errorCase = true;
      gErrorMessage.append( "ERROR (LET format)" );
      return;
    } // if

    else if ( walk2->mStr.compare( "(" ) == 0
              && walk2->mLeft->mStr.compare( "(" ) != 0 ) {
      errorCase = true;
      gErrorMessage.append( "ERROR (LET format)" );
      return;
    } // else if

    else {

      for ( walk3 = walk2 ; walk3->mStr.compare( "nil" ) != 0 ; walk3 = walk3->mRight ) {
        walk4 = walk3->mLeft;
        if ( walk4->mRight->mStr.compare( "nil" ) == 0
             || walk4->mRight->mRight->mStr.compare( "nil" ) != 0 ) {
          errorCase = true;
          gErrorMessage.append( "ERROR (LET format)" );
          return;
        } // if

        else {
          if ( walk4->mLeft->mType == SYMBOL ) ;
          else if ( walk4->mLeft->mType == OURORDER ) {
            TravelTreeInLet(  resultSExp, walk4->mLeft->mStr );
          } // else if

          else {
            errorCase = true;
            gErrorMessage.append( "ERROR (LET format)" );
            return;
          } // else
        } // else
      } // for

      for ( walk3 = walk2 ; walk3->mStr.compare( "nil" ) != 0 ; walk3 = walk3->mRight ) {
        walk4 = walk3->mLeft;
        temp.str = walk4->mLeft->mStr;
        temp.type = walk4->mLeft->mType;

        if ( temp.type == SYMBOL ) {
          temp.memorySpaceNumber = gMemoryNumber;

          walk4 = walk4->mRight;

          temp.root = walk4->mLeft;

          if ( temp.root->mType == SYMBOL ) {
            exist = false;
            for ( int i = 0; i < gDefinition.size() && !exist ; i++ ) {
              if ( gDefinition[i].str.compare( temp.root->mStr ) == 0 ) {
                exist = true;
                CopyTree( temp.root, gDefinition[i].root );
                temp.memorySpaceNumber = gDefinition[i].memorySpaceNumber;
              } // if
            } // for

            if ( !exist ) {
              errorCase = true;
              sprintf( buffer, "ERROR (unbound symbol) : %s", temp.root->mStr.c_str() );
              gErrorMessage.append( buffer );
              return;
            } // if
          } // if

          else {
            EvalSExp( temp.root, temp.root, errorCase );
          } // else

          if ( !errorCase ) {
            gDefinition.insert( gDefinition.begin(), temp );
            localVariableNumber++, gMemoryNumber++;
          } // if
        } // if

        else {
          errorCase = true;
          gErrorMessage.append( "ERROR (LET format)" );
          return;
        } // else

      } // for

    } // else

    if ( !errorCase ) {
      walk = walk->mRight;
      for ( walk ; walk->mStr.compare( "nil" ) != 0 ; walk = walk->mRight ) {
        walk4 = walk->mLeft;
        EvalSExp( walk4, walk4, errorCase );
        answer = walk4;
      } // for

      resultSExp = answer;

      while ( localVariableNumber > 0 ) {
        gDefinition.erase( gDefinition.begin() );
        localVariableNumber--;
      } // while

    } // if

  } // else

} // Let()

void Lambda( TreePtr &resultSExp, bool &errorCase ) {
  Eval temp;
  Para store;
  int countParameter = 0;
  TreePtr walk = NULL, answer = NULL;
  gErrorMessage.clear();

  if ( resultSExp->mRight->mStr.compare( "nil" ) == 0
       || resultSExp->mRight->mRight->mStr.compare( "nil" ) == 0 ) {
    errorCase = true;
    gErrorMessage.append( "ERROR (lambda format)" );
    return;
  } // if

  else {
    walk = resultSExp->mRight;    // check first argument
    if ( walk->mLeft->mType != LEFT_PAREN && walk->mLeft->mType != NIL ) {
      errorCase = true;
      gErrorMessage.append( "ERROR (lambda format)" );
      return;
    } // if

    else {

      for ( TreePtr walker = walk->mLeft ; walker->mStr.compare( "nil" ) != 0 ; walker = walker->mRight ) {
        if ( walker->mLeft->mType != SYMBOL && walker->mLeft->mType != OURORDER ) {
          errorCase = true;
          gErrorMessage.append( "ERROR (lambda format)" );
          return;
        } // if

        else countParameter++;

      } // for

      if ( gTopLevel ) {
        gTopLevel = false;
        resultSExp->mStr = "#function", resultSExp->mType = ORDER ;
        resultSExp->mLeft = NULL, resultSExp->mRight = NULL;
      } // if

      else {
        if ( gDefineExist ) {
          gParameterNumber = countParameter;

          for ( TreePtr walker = walk->mLeft ; walker->mStr.compare( "nil" ) != 0
                   ; walker = walker->mRight ) {
            store.str = walker->mLeft->mStr;
            gParameter.push_back( store );
          } // for

          resultSExp = walk->mRight->mLeft;
        } // if

        else {
          if ( countParameter != gArgumentNumber ) {
            errorCase = true;
            gErrorMessage.append( "ERROR (incorrect number of arguments) : lambda expression" );
            return;
          } // if

          else {
            for ( TreePtr walker = walk->mLeft ; walker->mStr.compare( "nil" ) != 0
                     ; walker = walker->mRight ) {
              temp.str = walker->mLeft->mStr;
              CopyTree( temp.root, gParameter.front().root );
              gParameter.erase( gParameter.begin() );
              gDefinition.insert( gDefinition.begin(), temp );
            } // for
          } // else

          for ( TreePtr walker = walk->mRight ; walker->mStr.compare( "nil" ) != 0
                && !errorCase ; walker = walker->mRight ) {
            EvalSExp( walker->mLeft, answer, errorCase );
          } // for

          if ( !errorCase ) {
            gAnswerExist = true, resultSExp = answer;

            while ( countParameter > 0 ) {
              gDefinition.erase( gDefinition.begin() );
              countParameter--;
            } // while

          } // if
        } // else
      } // else
    } // else
  } // else

} // Lambda()

void TravelTree( TreePtr &root ) {
  bool jump = false;

  if ( root != NULL ) {
    for ( int j = 0 ; j < gDefinition.size() ; j++ ) {
      if ( gDefinition[j].str.compare( root->mStr ) == 0 ) {
        root->mType = SYMBOL;
        jump = true;
      } // if
    } // for

    for ( int i = 0  ; i < gFunction.size() && !jump ; i++ ) {
      if ( gFunction[i].str.compare( root->mStr ) == 0 ) {
        root->mType = OURORDER;
      } // if
    } // for

    TravelTree( root->mLeft );
    TravelTree( root->mRight );
  } // if

} // TravelTree()

void OurOrder( TreePtr &resultSExp, bool &errorCase ) {
  TreePtr walk = NULL;
  Eval temp;
  int countParameter = 0;
  char buffer[ 200 ];
  gErrorMessage.clear();

  for ( int i = 0 ; i < gFunction.size() && !errorCase ; i++ ) {
    if ( gFunction[i].str.compare( resultSExp->mLeft->mStr ) == 0 ) {
      if ( gFunction[i].function->mType == ORDER ) {
        gErrorFunctionName = resultSExp->mLeft->mStr;
        resultSExp->mLeft->mStr = gFunction[i].function->mStr;
        EvalSExp( resultSExp, resultSExp, errorCase );
        return;
      } // if

      else {
        for ( walk = resultSExp->mRight ; walk->mStr.compare( "nil" ) != 0 ; walk = walk->mRight ) {
          countParameter++;
        } // for

        if ( countParameter != gFunction[i].parameterNumber ) {
          errorCase = true;
          sprintf( buffer, "ERROR (incorrect number of arguments) : %s", resultSExp->mLeft->mStr.c_str() );
          gErrorMessage.append( buffer );
        } // if

        else {
          walk = resultSExp->mRight;
          for ( int j = 0 ; j < gFunction[i].parameter.size()
                && walk->mStr.compare( "nil" ) != 0 ; j++ ) {
            temp.str = gFunction[i].parameter[j];
            EvalSExp( walk->mLeft, walk->mLeft, errorCase );
            CopyTree(  temp.root, walk->mLeft );
            gDefinition.insert( gDefinition.begin(), temp );
            walk = walk->mRight;
          } // for

          CopyTree( resultSExp, gFunction[i].function );
          TravelTree( resultSExp );
          EvalSExp( resultSExp, resultSExp, errorCase );
          for ( int k = 0 ; k < gFunction[i].parameterNumber ; k++ ) {
            gDefinition.erase( gDefinition.begin() );
          } // for

          return;

        } // else

      } // else

    } // if

  } // for

} // OurOrder()

void QuoteTree( TreePtr &root ) {
  if ( root != NULL ) {
    if ( root->mType == INT || root->mType == FLOAT
         || root->mType == SYMBOL || root->mType == STRING ) {
      root->mQuoteMember = true;
    } // if

    QuoteTree( root->mLeft );
    QuoteTree( root->mRight );
  } // if

} // QuoteTree()

void Quote( TreePtr &resultSExp, int &count, bool &errorCase ) {
  gTopLevel = false;
  resultSExp = resultSExp->mRight->mLeft;
  QuoteTree( resultSExp );
} // Quote()

void Create_Error_Object( TreePtr &resultSExp, bool &errorCase ) {
  if ( !gErrorMessage.empty() ) resultSExp->mStr = gErrorMessage;
  else resultSExp->mStr = resultSExp->mRight->mLeft->mStr;

  if ( resultSExp->mStr[0] != '"' && resultSExp->mStr[resultSExp->mStr.size()-1] != '"'  ) {
    resultSExp->mStr.insert( resultSExp->mStr.begin(), '"' );
    resultSExp->mStr.insert( resultSExp->mStr.end(), '"' );
  } // if

  resultSExp->mType = ERROR;
  resultSExp->mLeft = NULL, resultSExp->mRight = NULL;
} // Create_Error_Object()

void Error_Object( TreePtr &resultSExp, bool &errorCase ) {
  EvalSExp(  resultSExp->mRight->mLeft,  resultSExp->mRight->mLeft, errorCase );
  if ( !errorCase ) {
    if ( resultSExp->mRight->mLeft->mType == ERROR ) {
      resultSExp->mStr = "#t";
      resultSExp->mType = T;
      resultSExp->mLeft = NULL, resultSExp->mRight = NULL;
    } // if

    else {
      resultSExp->mStr = "nil";
      resultSExp->mType = NIL;
      resultSExp->mLeft = NULL, resultSExp->mRight = NULL;
    } // else

  } // if
} // Error_Object()

void Read( TreePtr &resultSExp, bool &errorCase ) {
  int returnVal = 0;
  bool error = false;
  gChangeLine = false;
  ReadSExp( resultSExp, returnVal, error );
  if ( error ) {
    resultSExp = new Tree;
    Create_Error_Object( resultSExp, errorCase );
  } // if
} // Read()

void Write( TreePtr &resultSExp, bool &errorCase ) {
  int count = 0;
  bool enterCheck = false;
  gWriteExist = true;
  resultSExp = resultSExp->mRight->mLeft;
  EvalSExp( resultSExp, resultSExp, errorCase );
  Print( resultSExp, count, enterCheck );
  gWriteExist = false;
} // Write()

void Display_String( TreePtr &resultSExp, bool &errorCase ) {
  string temp = "";
  EvalSExp(  resultSExp->mRight->mLeft,  resultSExp->mRight->mLeft, errorCase );
  if ( resultSExp->mRight->mLeft->mType == STRING || resultSExp->mRight->mLeft->mType == ERROR ) {
    temp = resultSExp->mRight->mLeft->mStr;
    temp.erase( temp.begin() );
    temp.erase( temp.size()-1 );
    printf( "%s", temp.c_str() );
    resultSExp = resultSExp->mRight->mLeft;
  } // if

  else if ( resultSExp->mRight->mLeft->mType == SYMBOL ) {
    CheckBound( resultSExp->mRight->mLeft, errorCase );
    temp = resultSExp->mRight->mLeft->mStr;
    temp.erase( temp.begin() );
    temp.erase( temp.size()-1 );
    printf( "%s", temp.c_str() );
    resultSExp = resultSExp->mRight->mLeft;
  } // else if

} // Display_String()

void NewLine( TreePtr &resultSExp ) {
  printf( "\n" );
  resultSExp->mStr = "nil";
  resultSExp->mType = NIL;
  resultSExp->mLeft = NULL, resultSExp->mRight = NULL;
} // NewLine()

void EvaL( TreePtr &resultSExp, bool &errorCase ) {
  resultSExp = resultSExp->mRight->mLeft;
  EvalSExp( resultSExp, resultSExp, errorCase );
  EvalSExp( resultSExp, resultSExp, errorCase );
} // EvaL()

void Set( TreePtr &resultSExp, bool &errorCase ) {
  Eval temp;
  Fun tempFunction;
  TreePtr walk = NULL;

  if ( resultSExp->mRight->mStr.compare( "nil" ) == 0
       || resultSExp->mRight->mRight->mStr.compare( "nil" ) == 0
       || resultSExp->mRight->mRight->mRight->mStr.compare( "nil" ) != 0 ) {
    errorCase = true;
    gErrorMessage.append( "ERROR (set! format)" );
    return;
  } // if

  else {
    gTopLevel = false;
    walk = resultSExp->mRight;

    if ( walk->mLeft->mType == SYMBOL || walk->mLeft->mType == OURORDER ) {

      if ( walk->mRight->mLeft->mStr.compare( "(" ) == 0
           && walk->mRight->mLeft->mLeft->mStr.compare( "lambda" ) == 0  ) {
        gDefineExist = true;
        EvalSExp( walk->mRight->mLeft, walk->mRight->mLeft, errorCase );
        tempFunction.str = walk->mLeft->mStr;
        tempFunction.parameterNumber = gParameterNumber;
        CopyTree( tempFunction.function, walk->mRight->mLeft );

        for ( int i = 0  ; i < gParameterNumber ; i++ ) {
          tempFunction.parameter.push_back( gParameter[i].str );
        } // for

        for ( int j = 0  ; j < gFunction.size() ; j++ ) {
          if ( gFunction[j].str.compare( tempFunction.str ) == 0 ) {
            gFunction.erase( gFunction.begin()+j );
          } // if
        } // for

        for ( int j = 0  ; j < gDefinition.size() ; j++ ) {
          if ( gDefinition[j].str.compare( tempFunction.str ) == 0 ) {
            gDefinition.erase( gDefinition.begin()+j );
          } // if
        } // for

        gFunction.push_back( tempFunction );
        resultSExp->mStr = "nil", resultSExp->mType = NIL;
        resultSExp->mLeft = NULL, resultSExp->mRight = NULL;
      } // if

      else if (  walk->mRight->mLeft->mType == ORDER ) {
        tempFunction.str = walk->mLeft->mStr;
        CopyTree( tempFunction.function, walk->mRight->mLeft );
        tempFunction.parameterNumber = 0;

        for ( int j = 0  ; j < gFunction.size() ; j++ ) {
          if ( gFunction[j].str.compare( tempFunction.str ) == 0 ) {
            gFunction.erase( gFunction.begin()+j );
          } // if
        } // for

        for ( int j = 0  ; j < gDefinition.size() ; j++ ) {
          if ( gDefinition[j].str.compare( tempFunction.str ) == 0 ) {
            gDefinition.erase( gDefinition.begin()+j );
          } // if
        } // for

        gFunction.push_back( tempFunction );
        resultSExp->mStr = "nil", resultSExp->mType = NIL;
        resultSExp->mLeft = NULL, resultSExp->mRight = NULL;
      } // else if

      else if (  walk->mRight->mLeft->mType == OURORDER ) {
        for ( int i = 0 ; i < gFunction.size() ; i++ ) {
          if ( gFunction[i].str.compare(  walk->mRight->mLeft->mStr ) == 0 ) {
            tempFunction = gFunction[i];
          } // if
        } // for

        for ( int j = 0  ; j < gFunction.size() ; j++ ) {
          if ( gFunction[j].str.compare( walk->mLeft->mStr ) == 0 ) {
            gFunction.erase( gFunction.begin()+j );
          } // if
        } // for

        for ( int j = 0  ; j < gDefinition.size() ; j++ ) {
          if ( gDefinition[j].str.compare( walk->mLeft->mStr ) == 0 ) {
            gDefinition.erase( gDefinition.begin()+j );
          } // if
        } // for

        tempFunction.str = walk->mLeft->mStr;
        gFunction.push_back( tempFunction );
        resultSExp->mStr = "nil", resultSExp->mType = NIL;
        resultSExp->mLeft = NULL, resultSExp->mRight = NULL;
      } // else if

      else {
        temp.str = walk->mLeft->mStr;
        temp.type = walk->mLeft->mType;
        temp.memorySpaceNumber = gMemoryNumber;

        walk = walk->mRight;
        temp.root = walk->mLeft;

        if ( temp.root->mType == SYMBOL ) {
          for ( int i = 0; i < gDefinition.size() ; i++ ) {
            if ( gDefinition[i].str.compare( temp.root->mStr ) == 0 ) {
              temp.root = gDefinition[i].root;
              temp.memorySpaceNumber = gDefinition[i].memorySpaceNumber;
            } // if
          } // for
        } // if

        else {
          EvalSExp( temp.root, temp.root, errorCase );
        } // else

        if ( !errorCase ) {
          for ( int j = 0; j < gDefinition.size() ; j++ ) {
            if ( gDefinition[j].str.compare( temp.str ) == 0 ) {
              gDefinition.erase( gDefinition.begin()+j );
            } // if
          } // for

          for ( int j = 0  ; j < gFunction.size() ; j++ ) {
            if ( gFunction[j].str.compare( temp.str ) == 0 ) {
              gFunction.erase( gFunction.begin()+j );
            } // if
          } // for

          if ( temp.root->mType == ERROR ) temp.type = ERROR;
          gDefinition.push_back( temp );
          resultSExp->mStr = "nil", resultSExp->mType = NIL;
          resultSExp->mLeft = NULL, resultSExp->mRight = NULL;
          gMemoryNumber++;
        } // if
      } // else

    } // if

    else {
      errorCase = true;
      gErrorMessage.append( "ERROR (set! format)" );
    } // else

  } // else
} // Set()

void Car( TreePtr &resultSExp, bool &errorCase ) {
  char buffer[ 200 ];
  gErrorMessage.clear();


  if ( resultSExp->mRight->mStr.compare( "nil" ) == 0
       || resultSExp->mRight->mRight->mStr.compare( "nil" ) != 0 ) {
    errorCase = true;
    if ( gErrorFunctionName.compare( "" ) != 0 ) {
      sprintf( buffer, "ERROR (incorrect number of arguments) : %s", gErrorFunctionName.c_str() );
      gErrorMessage.append( buffer );
    } // if

    else gErrorMessage.append( "ERROR (incorrect number of arguments) : car" );
    return;
  } // if

  else {
    gTopLevel = false;
    EvalSExp( resultSExp->mRight->mLeft, resultSExp->mRight->mLeft, errorCase );
    if ( !errorCase ) {
      if ( resultSExp != NULL  && resultSExp->mRight->mLeft->mLeft != NULL ) {
        resultSExp = resultSExp->mRight->mLeft->mLeft;
        if ( resultSExp->mStr.compare( "(" ) == 0 ) resultSExp->mDot = false;
        else resultSExp->mPosition = MIDDLE;
      } // if

      else {
        if ( !errorCase ) {
          errorCase = true, gErrorPointer = NULL;
          gErrorMessage.append( "ERROR (car with incorrect argument type) : " );
          CopyTree( gErrorPointer, resultSExp->mRight->mLeft );
          return;
        } // if
      } // else
    } // if

  } // else

} // Car()

void Cdr( TreePtr &resultSExp, bool &errorCase ) {
  char buffer[ 200 ];
  gErrorMessage.clear();

  if ( resultSExp->mRight->mStr.compare( "nil" ) == 0
       || resultSExp->mRight->mRight->mStr.compare( "nil" ) != 0 ) {
    errorCase = true;
    if ( gErrorFunctionName.compare( "" ) != 0 ) {
      sprintf( buffer, "ERROR (incorrect number of arguments) : %s", gErrorFunctionName.c_str() );
      gErrorMessage.append( buffer );
    } // if

    else gErrorMessage.append( "ERROR (incorrect number of arguments) : cdr" );
    return;
  } // if

  else {
    gTopLevel = false;
    EvalSExp( resultSExp->mRight->mLeft, resultSExp->mRight->mLeft, errorCase );
    if ( resultSExp != NULL  && resultSExp->mRight->mLeft->mRight != NULL ) {
      resultSExp = resultSExp->mRight->mLeft->mRight;
      if ( resultSExp->mStr.compare( "(" ) == 0 ) resultSExp->mDot = false;
      else resultSExp->mPosition = MIDDLE;
    } // if

    else {
      if ( !errorCase ) {
        errorCase = true, gErrorPointer = NULL;
        gErrorMessage.append( "ERROR (cdr with incorrect argument type) : " );
        CopyTree( gErrorPointer, resultSExp->mRight->mLeft );
        return;
      } // if
    } // else

  } // else

} // Cdr()

void Pair( TreePtr &resultSExp, bool &errorCase ) {
  char buffer[ 200 ];
  gErrorMessage.clear();

  if ( resultSExp->mRight->mStr.compare( "nil" ) == 0
       || resultSExp->mRight->mRight->mStr.compare( "nil" ) != 0 ) {
    errorCase = true;
    if ( gErrorFunctionName.compare( "" ) != 0 ) {
      sprintf( buffer, "ERROR (incorrect number of arguments) : %s", gErrorFunctionName.c_str() );
      gErrorMessage.append( buffer );
    } // if

    else gErrorMessage.append( "ERROR (incorrect number of arguments) : pair?" );
    return;
  } // if

  else {
    gTopLevel = false;
    resultSExp = resultSExp->mRight->mLeft;
    EvalSExp( resultSExp, resultSExp, errorCase );
    if ( !errorCase ) {
      if ( resultSExp->mType == LEFT_PAREN ) {
        True( resultSExp );
      } // if

      else {
        False( resultSExp );
      } // else
    } // if
  } // else

} // Pair()

void Null( TreePtr &resultSExp, bool &errorCase ) {
  char buffer[ 200 ];
  gErrorMessage.clear();

  if ( resultSExp->mRight->mStr.compare( "nil" ) == 0
       || resultSExp->mRight->mRight->mStr.compare( "nil" ) != 0 ) {
    errorCase = true;
    if ( gErrorFunctionName.compare( "" ) != 0 ) {
      sprintf( buffer, "ERROR (incorrect number of arguments) : %s", gErrorFunctionName.c_str() );
      gErrorMessage.append( buffer );
    } // if

    else gErrorMessage.append( "ERROR (incorrect number of arguments ) : null?" );
    return;
  } // if

  else {
    gTopLevel = false;
    resultSExp = resultSExp->mRight->mLeft;
    EvalSExp( resultSExp, resultSExp, errorCase );
    if ( !errorCase ) {
      if ( resultSExp->mType == NIL ) {
        True( resultSExp );
      } // if

      else {
        False( resultSExp );
      } // else
    } // if
  } // else

} // Null()

void Integer( TreePtr &resultSExp, bool &errorCase ) {
  char buffer[ 200 ];
  gErrorMessage.clear();

  if ( resultSExp->mRight->mStr.compare( "nil" ) == 0
       || resultSExp->mRight->mRight->mStr.compare( "nil" ) != 0 ) {
    errorCase = true;
    if ( gErrorFunctionName.compare( "" ) != 0 ) {
      sprintf( buffer, "ERROR (incorrect number of arguments) : %s", gErrorFunctionName.c_str() );
      gErrorMessage.append( buffer );
    } // if

    else gErrorMessage.append( "ERROR (incorrect number of arguments) : integer?" );
    return;
  } // if

  else {
    gTopLevel = false;
    resultSExp = resultSExp->mRight->mLeft;
    EvalSExp( resultSExp, resultSExp, errorCase );
    if ( !errorCase ) {
      if ( resultSExp->mType == INT ) {
        True( resultSExp );
      } // if

      else {
        False( resultSExp );
      } // else
    } // if
  } // else

} // Integer()

void Real( TreePtr &resultSExp, bool &errorCase ) {
  char buffer[ 200 ];
  gErrorMessage.clear();

  if ( resultSExp->mRight->mStr.compare( "nil" ) == 0
       || resultSExp->mRight->mRight->mStr.compare( "nil" ) != 0 ) {
    errorCase = true;
    if ( gErrorFunctionName.compare( "" ) != 0 ) {
      sprintf( buffer, "ERROR (incorrect number of arguments) : %s", gErrorFunctionName.c_str() );
      gErrorMessage.append( buffer );
    } // if

    else gErrorMessage.append( "ERROR (incorrect number of arguments) : real?" );
    return;
  } // if

  else {
    gTopLevel = false;
    resultSExp = resultSExp->mRight->mLeft;
    EvalSExp( resultSExp, resultSExp, errorCase );
    if ( !errorCase ) {
      if ( resultSExp->mType == FLOAT ) {
        True( resultSExp );
      } // if

      else {
        False( resultSExp );
      } // else
    } // if
  } // else

} // Real()

void Number( TreePtr &resultSExp, bool &errorCase ) {
  char buffer[ 200 ];
  gErrorMessage.clear();

  if ( resultSExp->mRight->mStr.compare( "nil" ) == 0
       || resultSExp->mRight->mRight->mStr.compare( "nil" ) != 0 ) {
    errorCase = true;
    if ( gErrorFunctionName.compare( "" ) != 0 ) {
      sprintf( buffer, "ERROR (incorrect number of arguments) : %s", gErrorFunctionName.c_str() );
      gErrorMessage.append( buffer );
    } // if

    else gErrorMessage.append( "ERROR (incorrect number of arguments) : number?" );
    return;
  } // if

  else {
    gTopLevel = false;
    resultSExp = resultSExp->mRight->mLeft;
    EvalSExp( resultSExp, resultSExp, errorCase );
    if ( !errorCase ) {
      if ( resultSExp->mType == INT || resultSExp->mType == FLOAT ) {
        True( resultSExp );
      } // if

      else {
        False( resultSExp );
      } // else
    } // if
  } // else

} // Number()

void String( TreePtr &resultSExp, bool &errorCase ) {
  char buffer[ 200 ];
  gErrorMessage.clear();

  if ( resultSExp->mRight->mStr.compare( "nil" ) == 0
       || resultSExp->mRight->mRight->mStr.compare( "nil" ) != 0 ) {
    errorCase = true;
    if ( gErrorFunctionName.compare( "" ) != 0 ) {
      sprintf( buffer, "ERROR (incorrect number of arguments) : %s", gErrorFunctionName.c_str() );
      gErrorMessage.append( buffer );
    } // if

    else gErrorMessage.append( "ERROR (incorrect number of arguments) : string?" );
    return;
  } // if

  else {
    gTopLevel = false;
    resultSExp = resultSExp->mRight->mLeft;
    EvalSExp( resultSExp, resultSExp, errorCase );
    if ( !errorCase ) {
      if ( resultSExp->mType == STRING ) {
        True( resultSExp );
      } // if

      else {
        False( resultSExp );
      } // else
    } // if
  } // else

} // String()

void Boolean( TreePtr &resultSExp, bool &errorCase ) {
  char buffer[ 200 ];
  gErrorMessage.clear();

  if ( resultSExp->mRight->mStr.compare( "nil" ) == 0
       || resultSExp->mRight->mRight->mStr.compare( "nil" ) != 0 ) {
    errorCase = true;
    if ( gErrorFunctionName.compare( "" ) != 0 ) {
      sprintf( buffer, "ERROR (incorrect number of arguments) : %s", gErrorFunctionName.c_str() );
      gErrorMessage.append( buffer );
    } // if

    else gErrorMessage.append( "ERROR (incorrect number of arguments) : boolean?" );
    return;
  } // if

  else {
    gTopLevel = false;
    resultSExp = resultSExp->mRight->mLeft;
    EvalSExp( resultSExp, resultSExp, errorCase );
    if ( !errorCase ) {
      if ( resultSExp->mType == T || resultSExp->mType == NIL ) {
        True( resultSExp );
      } // if

      else {
        False( resultSExp );
      } // else
    } // if
  } // else

} // Boolean()

void Symbol( TreePtr &resultSExp, bool &errorCase ) {
  char buffer[ 200 ];
  gErrorMessage.clear();

  if ( resultSExp->mRight->mStr.compare( "nil" ) == 0
       || resultSExp->mRight->mRight->mStr.compare( "nil" ) != 0 ) {
    errorCase = true;
    if ( gErrorFunctionName.compare( "" ) != 0 ) {
      sprintf( buffer, "ERROR (incorrect number of arguments) : %s", gErrorFunctionName.c_str() );
      gErrorMessage.append( buffer );
    } // if

    else gErrorMessage.append( "ERROR (incorrect number of arguments) : symbol?" );
    return;
  } // if

  else {
    gTopLevel = false;
    resultSExp = resultSExp->mRight->mLeft;
    EvalSExp( resultSExp, resultSExp, errorCase );
    if ( !errorCase ) {
      if ( resultSExp->mType == SYMBOL ) {
        True( resultSExp );
      } // if

      else {
        False( resultSExp );
      } // else
    } // if
  } // else

} // Symbol()

void Result( TreePtr &resultSExp, double &fresult, bool &floatExist, bool &errorCase ) {
  if ( !errorCase ) {
    if ( floatExist ) {
      resultSExp->mFnum = fresult;
      resultSExp->mStr.clear();
      resultSExp->mType = FLOAT;
      resultSExp->mPosition = MIDDLE;
      resultSExp->mLeft = NULL, resultSExp->mRight = NULL;
    } // if

    else {
      resultSExp->mInum = ( int ) fresult;
      resultSExp->mStr.clear();
      resultSExp->mType = INT;
      resultSExp->mPosition = MIDDLE;
      resultSExp->mLeft = NULL, resultSExp->mRight = NULL;
    } // else

  } // if
} // Result()

void Plus( TreePtr &resultSExp, double &fresult,
                  bool &floatExist, bool &firstNumber, bool &errorCase ) {
  char buffer[ 200 ];
  gErrorMessage.clear();

  if ( resultSExp->mRight->mStr.compare( "nil" ) == 0
       || resultSExp->mRight->mRight->mStr.compare( "nil" ) == 0 ) {
    errorCase = true;
    if ( gErrorFunctionName.compare( "" ) != 0 ) {
      sprintf( buffer, "ERROR (incorrect number of arguments) : %s", gErrorFunctionName.c_str() );
      gErrorMessage.append( buffer );
    } // if

    else gErrorMessage.append( "ERROR (incorrect number of arguments) : +" );
    return;
  } // if

  else {
    gTopLevel = false;
    for ( TreePtr walk = resultSExp->mRight; walk->mStr.compare( "nil" ) != 0 ; walk = walk->mRight ) {
      EvalSExp( walk->mLeft, walk->mLeft, errorCase );
      if ( !errorCase ) {
        if ( walk->mLeft->mType == INT ) {
          if ( firstNumber ) fresult = walk->mLeft->mInum, firstNumber = false;
          else fresult = fresult + walk->mLeft->mInum;
        } // if

        else if ( walk->mLeft->mType == FLOAT ) {
          floatExist = true;
          if ( firstNumber ) fresult = walk->mLeft->mFnum, firstNumber = false;
          else fresult = fresult + walk->mLeft->mFnum;
        } // else if

        else {
          errorCase = true, gErrorPointer = NULL;
          gErrorMessage.append( "ERROR (+ with incorrect argument type) : " );
          CopyTree( gErrorPointer, walk->mLeft );
          return;
        } // else
      } // if
    } // for

  } // else

} // Plus()

void Minus( TreePtr &resultSExp, double &fresult,
                    bool &floatExist, bool &firstNumber, bool &errorCase ) {
  char buffer[ 200 ];
  gErrorMessage.clear();

  if ( resultSExp->mRight->mStr.compare( "nil" ) == 0
       || resultSExp->mRight->mRight->mStr.compare( "nil" ) == 0 ) {
    errorCase = true;
    if ( gErrorFunctionName.compare( "" ) != 0 ) {
      sprintf( buffer, "ERROR (incorrect number of arguments) : %s", gErrorFunctionName.c_str() );
      gErrorMessage.append( buffer );
    } // if

    else gErrorMessage.append( "ERROR (incorrect number of arguments) : -" );
    return;
  } // if

  else {
    gTopLevel = false, resultSExp = resultSExp->mRight;
    for ( TreePtr walk = resultSExp; walk->mStr.compare( "nil" ) != 0 ; walk = walk->mRight ) {
      EvalSExp( walk->mLeft, walk->mLeft, errorCase );
      if ( !errorCase ) {
        if ( walk->mLeft->mType == INT ) {
          if ( firstNumber ) fresult = walk->mLeft->mInum, firstNumber = false;
          else fresult = fresult - walk->mLeft->mInum;
        } // if

        else if ( walk->mLeft->mType == FLOAT ) {
          floatExist = true;
          if ( firstNumber ) fresult = walk->mLeft->mFnum, firstNumber = false;
          else fresult = fresult - walk->mLeft->mFnum;
        } // else if

        else {
          errorCase = true, gErrorPointer = NULL;
          gErrorMessage.append( "ERROR (- with incorrect argument type) : " );
          CopyTree( gErrorPointer, walk->mLeft );
          return;
        } // else
      } // if
    } // for

  } // else

} // Minus()

void Multiply( TreePtr &resultSExp, double &fresult,
                       bool &floatExist, bool &firstNumber, bool &errorCase ) {
  char buffer[ 200 ];
  gErrorMessage.clear();

  if ( resultSExp->mRight->mStr.compare( "nil" ) == 0
       || resultSExp->mRight->mRight->mStr.compare( "nil" ) == 0 ) {
    errorCase = true;
    if ( gErrorFunctionName.compare( "" ) != 0 ) {
      sprintf( buffer, "ERROR (incorrect number of arguments) : %s", gErrorFunctionName.c_str() );
      gErrorMessage.append( buffer );
    } // if

    else gErrorMessage.append( "ERROR (incorrect number of arguments) : *" );
    return;
  } // if

  else {
    gTopLevel = false, resultSExp = resultSExp->mRight;
    for ( TreePtr walk = resultSExp; walk->mStr.compare( "nil" ) != 0 ; walk = walk->mRight ) {
      EvalSExp( walk->mLeft, walk->mLeft, errorCase );
      if ( !errorCase ) {
        if ( walk->mLeft->mType == INT ) {
          if ( firstNumber ) fresult = walk->mLeft->mInum, firstNumber = false;
          else fresult = fresult * walk->mLeft->mInum;
        } // if

        else if ( walk->mLeft->mType == FLOAT ) {
          floatExist = true;
          if ( firstNumber ) fresult = walk->mLeft->mFnum, firstNumber = false;
          else fresult = fresult * walk->mLeft->mFnum;
        } // else if

        else {
          errorCase = true, gErrorPointer = NULL;
          gErrorMessage.append( "ERROR (* with incorrect argument type) : " );
          CopyTree( gErrorPointer, walk->mLeft );
          return;
        } // else
      } // if
    } // for

  } // else

} // Multiply()

void Divide( TreePtr &resultSExp, double &fresult,
                     bool &floatExist, bool &firstNumber, bool &errorCase ) {
  char buffer[ 200 ];
  gErrorMessage.clear();

  if ( resultSExp->mRight->mStr.compare( "nil" ) == 0
       || resultSExp->mRight->mRight->mStr.compare( "nil" ) == 0 ) {
    errorCase = true;
    if ( gErrorFunctionName.compare( "" ) != 0 ) {
      sprintf( buffer, "ERROR (incorrect number of arguments) : %s", gErrorFunctionName.c_str() );
      gErrorMessage.append( buffer );
    } // if

    else gErrorMessage.append( "ERROR (incorrect number of arguments) : /" );
    return;
  } // if

  else {
    gTopLevel = false, resultSExp = resultSExp->mRight;
    for ( TreePtr walk = resultSExp; walk->mStr.compare( "nil" ) != 0 ; walk = walk->mRight ) {
      EvalSExp( walk->mLeft, walk->mLeft, errorCase );
      if ( !errorCase ) {
        if ( walk->mLeft->mType == INT ) {
          if ( firstNumber ) fresult = walk->mLeft->mInum, firstNumber = false;
          else fresult = fresult / walk->mLeft->mInum;
        } // if

        else if ( walk->mLeft->mType == FLOAT ) {
          floatExist = true;
          if ( firstNumber ) fresult = walk->mLeft->mFnum, firstNumber = false;
          else fresult = fresult / walk->mLeft->mFnum;
        } // else if

        else {
          errorCase = true, gErrorPointer = NULL;
          gErrorMessage.append( "ERROR (/ with incorrect argument type) : " );
          CopyTree( gErrorPointer, walk->mLeft );
          return;
        } // else
      } // if
    } // for

  } // else

} // Divide()

void Not( TreePtr &resultSExp, bool &errorCase ) {
  char buffer[ 200 ];
  gErrorMessage.clear();

  if ( resultSExp->mRight->mStr.compare( "nil" ) == 0
       || resultSExp->mRight->mRight->mStr.compare( "nil" ) != 0 ) {
    errorCase = true;
    if ( gErrorFunctionName.compare( "" ) != 0 ) {
      sprintf( buffer, "ERROR (incorrect number of arguments) : %s", gErrorFunctionName.c_str() );
      gErrorMessage.append( buffer );
    } // if

    else gErrorMessage.append( "ERROR (incorrect number of arguments) : not" );
    return;
  } // if

  else {
    gTopLevel = false;
    resultSExp = resultSExp->mRight->mLeft;
    EvalSExp( resultSExp, resultSExp, errorCase );
    if ( resultSExp->mType == NIL ) {
      True( resultSExp );
    } // if

    else {
      False( resultSExp );
    } // else

  } // else

} // Not()

void And( TreePtr &resultSExp, bool &errorCase ) {
  char buffer[ 200 ];
  TreePtr temp1 = NULL, temp2 = NULL;
  gErrorMessage.clear();

  if ( resultSExp->mRight->mStr.compare( "nil" ) == 0
       || resultSExp->mRight->mRight->mStr.compare( "nil" ) == 0
       || resultSExp->mRight->mRight->mRight->mStr.compare( "nil" ) != 0 ) {
    errorCase = true;
    if ( gErrorFunctionName.compare( "" ) != 0 ) {
      sprintf( buffer, "ERROR (incorrect number of arguments) : %s", gErrorFunctionName.c_str() );
      gErrorMessage.append( buffer );
    } // if

    else gErrorMessage.append( "ERROR (incorrect number of arguments) : and" );
    return;
  } // if

  else {
    gTopLevel = false;
    temp1 = resultSExp->mRight->mLeft;
    EvalSExp( temp1, temp1, errorCase );
    temp2 = resultSExp->mRight->mRight->mLeft;
    EvalSExp( temp2, temp2, errorCase );
    if ( ( temp1->mType == T && temp2->mType == T )
         || ( temp1->mType != NIL && temp2->mType != NIL ) ) {
      True( resultSExp );
    } // if

    else {
      False( resultSExp );
    } // else

  } // else

} // And()

void Or( TreePtr &resultSExp, bool &errorCase ) {
  char buffer[ 200 ];
  TreePtr temp1 = NULL, temp2 = NULL;
  gErrorMessage.clear();

  if ( resultSExp->mRight->mStr.compare( "nil" ) == 0
       || resultSExp->mRight->mRight->mStr.compare( "nil" ) == 0
       || resultSExp->mRight->mRight->mRight->mStr.compare( "nil" ) != 0 ) {
    errorCase = true;
    if ( gErrorFunctionName.compare( "" ) != 0 ) {
      sprintf( buffer, "ERROR (incorrect number of arguments) : %s", gErrorFunctionName.c_str() );
      gErrorMessage.append( buffer );
    } // if

    else gErrorMessage.append( "ERROR (incorrect number of arguments) : or" );
    return;
  } // if

  else {
    gTopLevel = false;
    temp1 = resultSExp->mRight->mLeft;
    EvalSExp( temp1, temp1, errorCase );
    temp2 = resultSExp->mRight->mRight->mLeft;
    EvalSExp( temp2, temp2, errorCase );
    if ( temp1->mType != NIL || temp2->mType != NIL ) {
      True( resultSExp );
    } // if

    else {
      False( resultSExp );
    } // else

  } // else

} // Or()

void OutCome( TreePtr &resultSExp, bool &outcome, bool &errorCase ) {
  if ( !errorCase ) {
    if ( outcome ) True( resultSExp );

    else False( resultSExp );

  } // if

} // OutCome()

void Greater( TreePtr &resultSExp, double &number,
                  bool &outcome, bool &firstNumber, bool &errorCase ) {
  char buffer[ 200 ];
  gErrorMessage.clear();

  if ( resultSExp->mRight->mStr.compare( "nil" ) == 0
       || resultSExp->mRight->mRight->mStr.compare( "nil" ) == 0 ) {
    errorCase = true;
    if ( gErrorFunctionName.compare( "" ) != 0 ) {
      sprintf( buffer, "ERROR (incorrect number of arguments) : %s", gErrorFunctionName.c_str() );
      gErrorMessage.append( buffer );
    } // if

    else gErrorMessage.append( "ERROR (incorrect number of arguments) : >" );
    return;
  } // if

  else {
    gTopLevel = false, outcome = true, resultSExp = resultSExp->mRight;
    for ( TreePtr walk = resultSExp; walk->mStr.compare( "nil" ) != 0 ; walk = walk->mRight ) {
      EvalSExp( walk->mLeft, walk->mLeft, errorCase );
      if ( !errorCase && outcome ) {
        if ( walk->mLeft->mType == INT ) {
          if ( firstNumber ) {
            number = walk->mLeft->mInum;
            firstNumber = false;
          } // if

          else {
            if ( number > walk->mLeft->mInum ) outcome = true;
            else outcome = false;
            number = walk->mLeft->mInum;
          } // else
        } // if

        else if ( walk->mLeft->mType == FLOAT ) {
          if ( firstNumber ) {
            number = walk->mLeft->mFnum;
            firstNumber = false;
          } // if

          else {
            if ( number > walk->mLeft->mFnum ) outcome = true;
            else outcome = false;
            number = walk->mLeft->mFnum;
          } // else
        } // else if

        else {
          errorCase = true, gErrorPointer = NULL;
          gErrorMessage.append( "ERROR (> with incorrect argument type) : " );
          CopyTree( gErrorPointer, walk->mLeft );
          return;
        } // else
      } // if
    } // for
  } // else

} // Greater()

void GreaterOrEqual( TreePtr &resultSExp, double &number,
                     bool &outcome, bool &firstNumber, bool &errorCase ) {
  char buffer[ 200 ];
  gErrorMessage.clear();

  if ( resultSExp->mRight->mStr.compare( "nil" ) == 0
       || resultSExp->mRight->mRight->mStr.compare( "nil" ) == 0 ) {
    errorCase = true;
    if ( gErrorFunctionName.compare( "" ) != 0 ) {
      sprintf( buffer, "ERROR (incorrect number of arguments) : %s", gErrorFunctionName.c_str() );
      gErrorMessage.append( buffer );
    } // if

    else gErrorMessage.append( "ERROR (incorrect number of arguments) : >=" );
    return;
  } // if

  else {
    gTopLevel = false, outcome = true, resultSExp = resultSExp->mRight;
    for ( TreePtr walk = resultSExp; walk->mStr.compare( "nil" ) != 0 ; walk = walk->mRight ) {
      EvalSExp( walk->mLeft, walk->mLeft, errorCase );
      if ( !errorCase && outcome ) {
        if ( walk->mLeft->mType == INT ) {
          if ( firstNumber ) {
            number = walk->mLeft->mInum;
            firstNumber = false;
          } // if

          else {
            if ( number >= walk->mLeft->mInum ) outcome = true;
            else outcome = false;
            number = walk->mLeft->mInum;
          } // else
        } // if

        else if ( walk->mLeft->mType == FLOAT ) {
          if ( firstNumber ) {
            number = walk->mLeft->mFnum;
            firstNumber = false;
          } // if

          else {
            if ( number >= walk->mLeft->mFnum ) outcome = true;
            else outcome = false;
            number = walk->mLeft->mFnum;
          } // else
        } // else if

        else {
          errorCase = true, gErrorPointer = NULL;
          gErrorMessage.append( "ERROR (>= with incorrect argument type) : " );
          CopyTree( gErrorPointer, walk->mLeft );
          return;
        } // else
      } // if
    } // for
  } // else

} // GreaterOrEqual()

void Less( TreePtr &resultSExp, double &number,
                  bool &outcome, bool &firstNumber, bool &errorCase ) {
  char buffer[ 200 ];
  gErrorMessage.clear();

  if ( resultSExp->mRight->mStr.compare( "nil" ) == 0
       || resultSExp->mRight->mRight->mStr.compare( "nil" ) == 0 ) {
    errorCase = true;
    if ( gErrorFunctionName.compare( "" ) != 0 ) {
      sprintf( buffer, "ERROR (incorrect number of arguments) : %s", gErrorFunctionName.c_str() );
      gErrorMessage.append( buffer );
    } // if

    else gErrorMessage.append( "ERROR (incorrect number of arguments) : <" );
    return;
  } // if

  else {
    gTopLevel = false, outcome = true, resultSExp = resultSExp->mRight;
    for ( TreePtr walk = resultSExp; walk->mStr.compare( "nil" ) != 0 ; walk = walk->mRight ) {
      EvalSExp( walk->mLeft, walk->mLeft, errorCase );
      if ( !errorCase && outcome ) {
        if ( walk->mLeft->mType == INT ) {
          if ( firstNumber ) {
            number = walk->mLeft->mInum;
            firstNumber = false;
          } // if

          else {
            if ( number < walk->mLeft->mInum ) outcome = true;
            else outcome = false;
            number = walk->mLeft->mInum;
          } // else
        } // if

        else if ( walk->mLeft->mType == FLOAT ) {
          if ( firstNumber ) {
            number = walk->mLeft->mFnum;
            firstNumber = false;
          } // if

          else {
            if ( number < walk->mLeft->mFnum ) outcome = true;
            else outcome = false;
            number = walk->mLeft->mFnum;
          } // else
        } // else if

        else {
          errorCase = true, gErrorPointer = NULL;
          gErrorMessage.append( "ERROR (< with incorrect argument type) : " );
          CopyTree( gErrorPointer, walk->mLeft );
          return;
        } // else
      } // if
    } // for
  } // else

} // Less()

void LessOrEqual( TreePtr &resultSExp, double &number,
                  bool &outcome, bool &firstNumber, bool &errorCase ) {
  char buffer[ 200 ];
  gErrorMessage.clear();

  if ( resultSExp->mRight->mStr.compare( "nil" ) == 0
       || resultSExp->mRight->mRight->mStr.compare( "nil" ) == 0 ) {
    errorCase = true;
    if ( gErrorFunctionName.compare( "" ) != 0 ) {
      sprintf( buffer, "ERROR (incorrect number of arguments) : %s", gErrorFunctionName.c_str() );
      gErrorMessage.append( buffer );
    } // if

    else gErrorMessage.append( "ERROR (incorrect number of arguments) : <=" );
    return;
  } // if

  else {
    gTopLevel = false, outcome = true, resultSExp = resultSExp->mRight;
    for ( TreePtr walk = resultSExp; walk->mStr.compare( "nil" ) != 0 ; walk = walk->mRight ) {
      EvalSExp( walk->mLeft, walk->mLeft, errorCase );
      if ( !errorCase && outcome ) {
        if ( walk->mLeft->mType == INT ) {
          if ( firstNumber ) {
            number = walk->mLeft->mInum;
            firstNumber = false;
          } // if

          else {
            if ( number <= walk->mLeft->mInum ) outcome = true;
            else outcome = false;
            number = walk->mLeft->mInum;
          } // else
        } // if

        else if ( walk->mLeft->mType == FLOAT ) {
          if ( firstNumber ) {
            number = walk->mLeft->mFnum;
            firstNumber = false;
          } // if

          else {
            if ( number <= walk->mLeft->mFnum ) outcome = true;
            else outcome = false;
            number = walk->mLeft->mFnum;
          } // else
        } // else if

        else {
          errorCase = true, gErrorPointer = NULL;
          gErrorMessage.append( "ERROR (<= with incorrect argument type) : " );
          CopyTree( gErrorPointer, walk->mLeft );
          return;
        } // else
      } // if
    } // for
  } // else

} // LessOrEqual()

void Equal( TreePtr &resultSExp, double &number,
                  bool &outcome, bool &firstNumber, bool &errorCase ) {
  char buffer[ 200 ];
  gErrorMessage.clear();

  if ( resultSExp->mRight->mStr.compare( "nil" ) == 0
       || resultSExp->mRight->mRight->mStr.compare( "nil" ) == 0 ) {
    errorCase = true;
    if ( gErrorFunctionName.compare( "" ) != 0 ) {
      sprintf( buffer, "ERROR (incorrect number of arguments) : %s", gErrorFunctionName.c_str() );
      gErrorMessage.append( buffer );
    } // if

    else gErrorMessage.append( "ERROR (incorrect number of arguments) : =" );
    return;
  } // if

  else {
    gTopLevel = false, resultSExp = resultSExp->mRight;
    for ( TreePtr walk = resultSExp; walk->mStr.compare( "nil" ) != 0 ; walk = walk->mRight ) {
      EvalSExp( walk->mLeft, walk->mLeft, errorCase );
      if ( !errorCase ) {
        if ( walk->mLeft->mType == INT ) {
          if ( firstNumber ) {
            number = walk->mLeft->mInum;
            firstNumber = false;
          } // if

          else {
            if ( number == walk->mLeft->mInum ) outcome = true;
            else outcome = false;
            number = walk->mLeft->mInum;
          } // else
        } // if

        else if ( walk->mLeft->mType == FLOAT ) {
          if ( firstNumber ) {
            number = walk->mLeft->mFnum;
            firstNumber = false;
          } // if

          else {
            if ( number == walk->mLeft->mFnum ) outcome = true;
            else outcome = false;
            number = walk->mLeft->mFnum;
          } // else
        } // else if

        else {
          errorCase = true, gErrorPointer = NULL;
          gErrorMessage.append( "ERROR (= with incorrect argument type) : " );
          CopyTree( gErrorPointer, walk->mLeft );
          return;
        } // else
      } // if
    } // for
  } // else

} // Equal()

void StringAppendResult( TreePtr &resultSExp, string &appendResult, bool &errorCase ) {
  if ( !errorCase ) {
    appendResult.insert( appendResult.begin(), '"' );
    appendResult.insert( appendResult.end(), '"' );
    resultSExp->mStr = appendResult;
    resultSExp->mType = STRING;
    resultSExp->mPosition = MIDDLE;
    resultSExp->mLeft = NULL, resultSExp->mRight = NULL;
  } // if
} // StringAppendResult()

void StringAppend( TreePtr &resultSExp,
                                string &appendResult, bool &errorCase ) {
  char buffer[ 200 ];
  gErrorMessage.clear();

  if ( resultSExp->mRight->mStr.compare( "nil" ) == 0
       || resultSExp->mRight->mRight->mStr.compare( "nil" ) == 0 ) {
    errorCase = true;
    if ( gErrorFunctionName.compare( "" ) != 0 ) {
      sprintf( buffer, "ERROR (incorrect number of arguments) : %s", gErrorFunctionName.c_str() );
      gErrorMessage.append( buffer );
    } // if

    else gErrorMessage.append( "ERROR (incorrect number of arguments) : string-append" );
    return;
  } // if

  else {
    gTopLevel = false, resultSExp = resultSExp->mRight;
    for ( TreePtr walk = resultSExp; walk->mStr.compare( "nil" ) != 0 ; walk = walk->mRight ) {
      EvalSExp( walk->mLeft, walk->mLeft, errorCase );
      if ( !errorCase ) {
        if ( walk->mLeft->mType == STRING ) {
          appendResult.append( walk->mLeft->mStr.begin()+1, walk->mLeft->mStr.end()-1 );
        } // if

        else {
          errorCase = true, gErrorPointer = NULL;
          gErrorMessage.append( "ERROR (string-append with incorrect argument type) : " );
          CopyTree( gErrorPointer, walk->mLeft );
          return;
        } // else
      } // if
    } // for
  } // else

} // StringAppend()

void StringCompare( TreePtr &resultSExp, string &currentString,
                    bool &outcome, bool &firstString, bool &errorCase ) {
  char buffer[ 200 ];
  gErrorMessage.clear();

  if ( resultSExp->mRight->mStr.compare( "nil" ) == 0
       || resultSExp->mRight->mRight->mStr.compare( "nil" ) == 0 ) {
    errorCase = true;
    if ( gErrorFunctionName.compare( "" ) != 0 ) {
      sprintf( buffer, "ERROR (incorrect number of arguments) : %s", gErrorFunctionName.c_str() );
      gErrorMessage.append( buffer );
    } // if

    else gErrorMessage.append( "ERROR (incorrect number of arguments) : string>?" );
    return;
  } // if

  else {
    gTopLevel = false, resultSExp = resultSExp->mRight;
    for ( TreePtr walk = resultSExp; walk->mStr.compare( "nil" ) != 0 ; walk = walk->mRight ) {
      EvalSExp( walk->mLeft, walk->mLeft, errorCase );
      if ( !errorCase ) {
        if ( walk->mLeft->mType == STRING || walk->mLeft->mType == ERROR ) {
          if ( firstString ) {
            currentString = walk->mLeft->mStr;
            firstString = false;
          } // if

          else {
            if ( currentString > walk->mLeft->mStr ) outcome = true;
            else outcome = false;
            currentString = walk->mLeft->mStr;
          } // else
        } // if

        else {
          errorCase = true, gErrorPointer = NULL;
          gErrorMessage.append( "ERROR (string>? with incorrect argument type) : " );
          CopyTree( gErrorPointer, walk->mLeft );
          return;
        } // else
      } // if
    } // for
  } // else

} // StringCompare()

void Eqv( TreePtr &resultSExp, bool&errorCase ) {
  int tempMemoryNumber1 = 0, tempMemoryNumber2 = 0;
  char buffer[ 200 ];
  TreePtr temp1 = NULL, temp2 = NULL;
  gErrorMessage.clear();

  if ( resultSExp->mRight->mStr.compare( "nil" ) == 0
       || resultSExp->mRight->mRight->mStr.compare( "nil" ) == 0
       || resultSExp->mRight->mRight->mRight->mStr.compare( "nil" ) != 0 ) {
    errorCase = true;
    if ( gErrorFunctionName.compare( "" ) != 0 ) {
      sprintf( buffer, "ERROR (incorrect number of arguments) : %s", gErrorFunctionName.c_str() );
      gErrorMessage.append( buffer );
    } // if

    else gErrorMessage.append( "ERROR (incorrect number of arguments) : eqv?" );
    return;
  } // if

  else {
    temp1 = resultSExp->mRight->mLeft;
    temp2 = resultSExp->mRight->mRight->mLeft;

    if ( temp1->mType == temp2->mType &&
         ( temp1->mType == INT || temp1->mType == FLOAT
           || temp1->mType == SYMBOL  || temp1->mType == T
           || temp1->mType == NIL  ) ) {

      if ( temp1->mType == SYMBOL ) {
        EvalSExp( temp1, temp1, errorCase );
        tempMemoryNumber1 = gTempMemoryNumber;
        EvalSExp( temp2, temp2, errorCase );
        tempMemoryNumber2 = gTempMemoryNumber;

        if ( tempMemoryNumber1 == tempMemoryNumber2 ) {
          True( resultSExp );
        } // if

        else {
          False( resultSExp );
        } // else
      } // if

      else {
        if ( temp1->mType == INT ) {
          if ( temp1->mInum == temp2->mInum ) True( resultSExp );
          else False( resultSExp );
        } // if

        else if ( temp1->mType == FLOAT ) {
          if ( temp1->mFnum == temp2->mFnum ) True( resultSExp );
          else False( resultSExp );
        } // else if

        else {
          if ( temp1->mStr.compare( temp2->mStr ) == 0 ) True( resultSExp );
          else False( resultSExp );
        } // else

      } // else

    } // if

    else {
      False( resultSExp );
    } // else

  } // else

} // Eqv()

void CompareTree( TreePtr root1, TreePtr root2, bool &same ) {
  if ( root1 != NULL && root2 != NULL && same ) {

    if ( root1->mType == root2->mType
         || root1->mType == ERROR || root2->mType == ERROR   ) {
      if ( root1->mType == INT && root2->mType == INT ) {
        if ( root1->mInum == root2->mInum ) same = true;
        else same = false;
      } // if

      else if ( root1->mType == FLOAT && root2->mType == FLOAT ) {
        if ( root1->mFnum == root2->mFnum ) same = true;
        else same = false;
      } // else if

      else {
        if ( root1->mStr.compare( root2->mStr ) == 0 ) same = true;
        else same = false;
      } // else

    } // if

    else {
      same = false;
    } // else

    CompareTree( root1->mLeft, root2->mLeft, same );
    CompareTree( root1->mRight, root2->mRight, same );
  } // if
} // CompareTree()

void EquaL( TreePtr &resultSExp, bool&errorCase ) {
  TreePtr temp1 = NULL, temp2 = NULL;
  char buffer[ 200 ];
  bool same = true;
  gErrorMessage.clear();

  if ( resultSExp->mRight->mStr.compare( "nil" ) == 0
       || resultSExp->mRight->mRight->mStr.compare( "nil" ) == 0
       || resultSExp->mRight->mRight->mRight->mStr.compare( "nil" ) != 0 ) {
    errorCase = true;
    if ( gErrorFunctionName.compare( "" ) != 0 ) {
      sprintf( buffer, "ERROR (incorrect number of arguments) : %s", gErrorFunctionName.c_str() );
      gErrorMessage.append( buffer );
    } // if

    else gErrorMessage.append( "ERROR (incorrect number of arguments) : equal?" );
    return;
  } // if

  else {
    temp1 = resultSExp->mRight->mLeft;
    EvalSExp( temp1, temp1, errorCase );
    temp2 = resultSExp->mRight->mRight->mLeft;
    EvalSExp( temp2, temp2, errorCase );
    CompareTree( temp1, temp2, same );
    if ( same ) True( resultSExp );
    else False( resultSExp );
  } // else

} // EquaL()

void Begin( TreePtr &resultSExp, bool&errorCase ) {
  char buffer[ 200 ];
  TreePtr answer = NULL;
  gErrorMessage.clear();

  if ( resultSExp->mRight->mStr.compare( "nil" ) == 0 ) {
    errorCase = true;
    if ( gErrorFunctionName.compare( "" ) != 0 ) {
      sprintf( buffer, "ERROR (incorrect number of arguments) : %s", gErrorFunctionName.c_str() );
      gErrorMessage.append( buffer );
    } // if

    else gErrorMessage.append( "ERROR (incorrect number of arguments) : begin" );
    return;
  } // if

  else {
    gTopLevel = false, resultSExp = resultSExp->mRight;
    for ( TreePtr walk = resultSExp; walk->mStr.compare( "nil" ) != 0 ; walk = walk->mRight ) {
      EvalSExp( walk->mLeft, walk->mLeft, errorCase );
      answer = walk->mLeft;
    } // for

    resultSExp = answer;

  } // else

} // Begin()

void If( TreePtr &resultSExp, bool&errorCase ) {
  char buffer[ 200 ];
  TreePtr statement = NULL, temp1 = NULL, temp2 = NULL;
  gErrorMessage.clear();

  if ( resultSExp->mRight->mStr.compare( "nil" ) == 0
       || resultSExp->mRight->mRight->mStr.compare( "nil" ) == 0
       || resultSExp->mRight->mRight->mRight->mStr.compare( "nil" ) == 0
       || resultSExp->mRight->mRight->mRight->mRight->mStr.compare( "nil" ) != 0 ) {
    errorCase = true;
    if ( gErrorFunctionName.compare( "" ) != 0 ) {
      sprintf( buffer, "ERROR (incorrect number of arguments) : %s", gErrorFunctionName.c_str() );
      gErrorMessage.append( buffer );
    } // if

    else gErrorMessage.append( "ERROR (incorrect number of arguments) : if" );
    return;
  } // if

  else {
    statement = resultSExp->mRight->mLeft;
    temp1 = resultSExp->mRight->mRight->mLeft;
    temp2 = resultSExp->mRight->mRight->mRight->mLeft;
    EvalSExp( statement, statement, errorCase );

    if ( !errorCase ) {
      if ( statement->mType == NIL ) {
        EvalSExp( temp2, resultSExp, errorCase );
      } // if

      else {
        EvalSExp( temp1, resultSExp, errorCase );
      } // else

    } // if

  } // else

} // If()

void Cond( TreePtr &resultSExp, bool&errorCase ) {
  char buffer[ 200 ];
  bool jump = false;
  TreePtr statement = NULL, outcome = NULL;
  TreePtr walk = NULL, walk2 = NULL, walk3 = NULL;
  gErrorMessage.clear();

  if ( resultSExp->mRight->mStr.compare( "nil" ) == 0 ) {
    errorCase = true;
    if ( gErrorFunctionName.compare( "" ) != 0 ) {
      sprintf( buffer, "ERROR (incorrect number of arguments) : %s", gErrorFunctionName.c_str() );
      gErrorMessage.append( buffer );
    } // if

    else gErrorMessage.append( "ERROR (incorrect number of arguments) : cond" );
    return;
  } // if

  else {
    for ( walk = resultSExp->mRight ; walk->mStr.compare( "nil" ) != 0 && !errorCase
             ; walk = walk->mRight ) {
      walk2 = walk->mLeft;
      if ( walk2->mLeft == NULL
           || walk2->mRight == NULL  ) {
        errorCase = true, gErrorPointer = NULL;
        gErrorMessage.append( "ERROR (cond parameter format) : " );
        CopyTree( gErrorPointer, walk2 );
        return;
      } // if


      else if ( walk2->mRight->mStr.compare( "nil" ) == 0  ) {
        errorCase = true, gErrorPointer = NULL;
        gErrorMessage.append( "ERROR (cond parameter format) : " );
        CopyTree( gErrorPointer, walk2 );
        return;
      } // else if
    } // for

    for ( walk = resultSExp->mRight ; walk->mStr.compare( "nil" ) != 0 && !errorCase
          && !jump ; walk = walk->mRight ) {
      walk2 = walk->mLeft;
      statement = walk2->mLeft;
      EvalSExp( statement, statement, errorCase );
      if ( !errorCase ) {
        if ( statement->mType == NIL ) ;
        else {
          for ( walk3 = walk2->mRight ; walk3->mStr.compare( "nil" ) != 0
                   ; walk3 = walk3->mRight ) {
            EvalSExp( walk3->mLeft, outcome, errorCase );
          } // for

          jump = true;
        } // else
      } // if
    } // for

    if ( !errorCase ) {
      if ( outcome == NULL ) {
        errorCase = true;
        gErrorMessage.append( "ERROR (no return value) : cond" );
        return;
      } // if

      else resultSExp = outcome;
    } // if

  } // else

} // Cond()

void CleanEnvironment( TreePtr &resultSExp, bool&errorCase ) {
  if ( gTopLevel ) {
    gDefinition.clear();
    resultSExp->mStr = "environment cleaned";
    resultSExp->mType = SYMBOL;
    resultSExp->mPosition = MIDDLE;
    resultSExp->mLeft = NULL, resultSExp->mRight = NULL;
  } // if

  else {
    errorCase = true, gErrorMessage.clear();
    gErrorMessage.append( "ERROR (clean-environment format)" );
  } // else

} // CleanEnvironment()

void Exit( TreePtr &resultSExp, bool &errorCase ) {
  if ( gTopLevel ) resultSExp = NULL, gExit = true;

  else {
    errorCase = true, gErrorMessage.clear();
    gErrorMessage.append( "ERROR (exit format)" );
  } // else

} // Exit()

void True( TreePtr &resultSExp ) {
  resultSExp->mStr = "#t";
  resultSExp->mType = T;
  resultSExp->mPosition = MIDDLE;
  resultSExp->mLeft = NULL, resultSExp->mRight = NULL;
} // True()

void False( TreePtr &resultSExp ) {
  resultSExp->mStr = "nil";
  resultSExp->mType = NIL;
  resultSExp->mPosition = MIDDLE;
  resultSExp->mLeft = NULL, resultSExp->mRight = NULL;
} // False()

void CheckBound( TreePtr &resultSExp, bool &errorCase ) {
  Eval temp;
  char buffer[ 200 ];
  bool exist = false;
  Position position;

  for ( int i = 0; i < gDefinition.size() && !exist ; i++ ) {
    if ( gDefinition[i].str.compare( resultSExp->mStr ) == 0 ) {
      exist = true;
      temp.str = gDefinition[i].str;
      gTempMemoryNumber = gDefinition[i].memorySpaceNumber;
      position = resultSExp->mPosition;
      CopyTree( temp.root, gDefinition[i].root );
      resultSExp = temp.root;

      if ( resultSExp->mStr.compare( "(" ) == 0 && position == RIGHT ) {
        resultSExp->mDot = true;
      } // if

      else resultSExp->mPosition = position;
    } // if
  } // for

  if ( !exist ) {
    errorCase = true, gErrorMessage.clear();
    sprintf( buffer, "ERROR (unbound symbol) : %s", resultSExp->mStr.c_str() );
    gErrorMessage.append( buffer );
  } // if

} // CheckBound()

void OurScheme() {
  string str = "";
  int returnVal = 0, count = 0;
  bool error = false, enterCheck = false, errorCase = false;
  TreePtr root = NULL, resultSExp = NULL;

  while ( returnVal != EOF && !gExit ) {
    printf( "> " );
    ReadSExp( root, returnVal, error );
    if ( !error ) {
      gTopLevel = true, gAnswerExist = false, gDefineExist = false, gErrorFunctionName = "";
      gArgumentNumber = 0, gParameterNumber = 0, gParameter.clear();
      EvalSExp( root, resultSExp, errorCase );
      gChangeLine = false;
      if ( resultSExp != NULL && !errorCase ) {
        Print( resultSExp, count, enterCheck );
      } // if

      else if ( errorCase ) {
        if ( gErrorPointer != NULL ) {
          printf( "%s", gErrorMessage.c_str() );
          Print( gErrorPointer, count, enterCheck );
        } // if

        else {
          printf( "%s\n", gErrorMessage.c_str() );
        } // else

        gErrorMessage.clear(), gErrorPointer = NULL;

      } // else if

    } // if

    else {
      printf( "%s\n", gErrorMessage.c_str() );
      gErrorMessage.clear(), gErrorPointer = NULL;
    } // else

    printf( "\n" ), error = false, errorCase = false;

  } // while

} // OurScheme()

int main() {
  char enter = '\0';
  scanf( "%d%c", &uTestNum, &enter );
  printf( "Welcome to OurScheme!\n\n" );
  OurScheme();
  printf( "Thanks for using OurScheme!" );
  return 0;
} // main()
