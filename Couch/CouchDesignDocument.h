//
//  CouchDesignDocument.h
//  CouchCocoa
//
//  Created by Jens Alfke on 6/8/11.
//  Copyright 2011 Couchbase, Inc.
//
//  Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file
//  except in compliance with the License. You may obtain a copy of the License at
//    http://www.apache.org/licenses/LICENSE-2.0
//  Unless required by applicable law or agreed to in writing, software distributed under the
//  License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
//  either express or implied. See the License for the specific language governing permissions
//  and limitations under the License.

#import "CouchDocument.h"
@class CouchQuery;


/** Language parameter for JavaScript map and reduce functions. */
extern NSString* const kCouchLanguageJavaScript;

/** Language parameter for Erlang map and reduce functions. */
extern NSString* const kCouchLanguageErlang;


/** A Design Document is a special document type that contains things like map/reduce functions, and the code and static resources for CouchApps. */
@interface CouchDesignDocument : CouchDocument
{
    @private
    NSMutableDictionary* _views;
    NSString* _viewsRevisionID;
    BOOL _changed;
}

/** Creates a query for the given named view.
    If view definitions have been modified but not saved yet, they will be saved first. */
- (CouchQuery*) queryViewNamed: (NSString*)viewName;

/** Fetches and returns the names of all the views defined in this design document.
    The first call fetches the entire design document synchronously; subsequent calls are cached. */
@property (readonly) NSArray* viewNames;

/** Returns the map function of the view with the given name. */
- (NSString*) mapFunctionOfViewNamed: (NSString*)viewName;

/** Returns the reduce function of the view with the given name. */
- (NSString*) reduceFunctionOfViewNamed: (NSString*)viewName;

/** Returns the language of the view with the given name. */
- (NSString*) languageOfViewNamed: (NSString*)viewName;

/** Sets the definition of a view, or deletes it.
    After making changes to one or more views, you should call -saveChanges to PUT them back to the database.
    If the new definition is identical to the existing one, the design document will not be marked as changed or saved back to the database.
    @param viewName  The name of the view, in the scope of this design doc.
    @param mapFunction  The source code of the map function. If nil, the view will be deleted.
    @param reduceFunction  The source code of the reduce function. Optional; pass nil for none.
    @param language  Specifies the language the functions are written in. If nil, defaults to kCouchLanguageJavaScript. */
- (void) defineViewNamed: (NSString*)viewName
                     map: (NSString*)mapFunction
                  reduce: (NSString*)reduceFunction
                language: (NSString*)language;

/** A shortcut that defines a simple JavaScript view with no reduce function.
    After making changes to one or more views, you should call -saveChanges to PUT them back to the database.
    If the new definition is identical to the existing one, the design document will not be marked as changed or saved back to the database.
    @param viewName  The name of the view, in the scope of this design doc.
    @param mapFunction  The source code of the map function. If nil, the view will be deleted. */
- (void) defineViewNamed: (NSString*)viewName
                     map: (NSString*)mapFunction;

/** Have the contents of the design document been changed in-memory but not yet saved? */
@property (readonly) BOOL changed;

/** Saves changes, asynchronously. If there are no current changes, returns nil. */
- (RESTOperation*) saveChanges;

@end
