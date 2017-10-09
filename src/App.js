import React, { Component } from 'react';
// Import Custom svg
import logo from './logo.svg';
import sanitizeHTML from 'sanitize-html';
import './App.css';
import http from 'axios';
import showdown from 'showdown';
import '../node_modules/github-fork-ribbon-css/gh-fork-ribbon.css';

// Create converter to create styled html from markdown
const converter = new showdown.Converter();
class App extends Component {
    constructor() {
        super();
        this.state = {
            username: 'bareinhard',
            repo: 'hacktoberfest-arduino',
            rootContent: [],
            rootDirectories: [],
            currentDirectory: [],
            displayCode: false,
            code: '',
            rootTree: [],
            codeContent: {},
        };
        this.loadFiles();
    }
    // Format Word to Reading Friendly format
    sanitizeWord = val => {
        val = this.removeUnderscore(val);
        return val;
    };
    // Helper function to remove underscores
    removeUnderscore = val => {
        val = val.replace(/_/g, ' ');
        val = this.properName(val);
        return val;
    };
    // Helper function to lower case all characters then create uppercase first character of each word
    properName = val => {
        val = val.toLowerCase();
        val = val.split(' ');
        val = val.map(word => word.substring(0, 1).toUpperCase() + word.substring(1));
        val = val.join(' ');
        return val;
    };
    // Get all root folders from contents API
    getRootFiles = async () => {
        let rootContent = await http
            .get(`https://api.github.com/repos/${this.state.username}/${this.state.repo}/contents/`)
            .then(res => res.data);
        let rootTree = [];
        let codeContent = {};
        // Get folders tree structure recursively
        for (let dir of rootContent) {
            if (dir.size === 0) {
                let treeObject = await http
                    .get(
                        `https://api.github.com/repos/${this.state.username}/${this.state
                            .repo}/git/trees/${dir.sha}?recursive=1`,
                    )
                    // Remove AJAX object and return data only
                    .then(res => res.data);
                // Create custom properties for easier manipulation
                treeObject.name = dir.name;
                treeObject.path = dir.path;
                treeObject.tree.forEach(files => {
                    codeContent[files.path] = '...';
                }),
                    rootTree.push(treeObject);
            }
        }
        // Create array of directories on root filepath
        let rootDirectories = rootContent.filter(file => file.size === 0);

        // Save all data as state
        this.setState({ rootContent, rootDirectories, rootTree, codeContent });
    };
    // Init function to get data
    loadFiles = async () => {
        this.getRootFiles('bareinhard', 'hacktoberfest-data-structure-and-algorithms');
    };
    // Click Listener function to get all subdirectories of specific
    // root directory
    clickDirectory = event => {
        let path = event.target.className;
        http
            .get(`https://api.github.com/repos/${this.state.username}/${this.state.repo}/contents/${path}?ref=master`)
            .then(res => {
                let newRoot = res.data;
                console.log(newRoot);
                let codeContent = {};
                newRoot.forEach(rootDir =>
                    rootDir.tree.forEach(files => {
                        codeContent[files.path] = '...';
                    }),
                );
                this.setState({ codeContent, currentDirectory: Array(...newRoot) });
            });
    };

    // Click handler to display subdirectories of root directory
    filterSubDirectories = event => {
        let path = event.target.id;
        let currentDirectory = this.state.rootTree.filter(dir => dir.sha === path);
        console.log(currentDirectory);
        currentDirectory = currentDirectory[0].tree.filter(dir => !dir.path.includes('/'));
        // Display contents for current root directory clicked
        this.setState({ currentDirectory });
    };
    // Click listener functon get file contents and display the code
    getContent = async path => {
        let code = await http
            .get(`https://api.github.com/repos/${this.state.username}/${this.state.repo}/contents/${path}?ref=master`)
            .then(res => res.data);
        let newPath = path.split('/');
        let finalPath = '';
        for (let sub of newPath) {
            if (sub === newPath[1] && sub !== newPath[0]) {
                finalPath += sub;
            } else if (sub !== newPath[0]) {
                finalPath += '/' + sub;
            }
        }
        console.log(finalPath, this.state.codeContent);
        // get encoded string and decode the BASE64 string
        this.setState({
            codeContent: Object.assign({}, this.state.codeContent, {
                [finalPath]: `data:image/png;base64,${code.content}`,
            }),
        });
    };
    loadCode = async event => {
        let path = event.target.id;
        let code = await http
            .get(`https://api.github.com/repos/${this.state.username}/${this.state.repo}/contents/${path}?ref=master`)
            .then(res => res.data);
        let newPath = path.split('/');
        let finalPath = '';
        for (let sub of newPath) {
            if (sub === newPath[1] && sub !== newPath[0]) {
                finalPath += sub;
            } else if (sub !== newPath[0]) {
                finalPath += '/' + sub;
            }
        }
        // get encoded string and decode the BASE64 string
        this.setState({
            codeContent: Object.assign({}, this.state.codeContent, { [finalPath]: atob(code.content) }),
        });
    };
    // Sanitize html returned from generated html from markdown conversion
    setMarkDown = html => {
        return { __html: converter.makeHtml(sanitizeHTML(html)) };
    };
    render() {
        return (
            <div className="App">
                {/* Set Title/Header with Logo and GitHub Ribbon */}
                <header className="App-header">
                    <a
                        className="github-fork-ribbon"
                        href={`https://github.com/${this.state.username}/${this.state.repo}`}
                        data-ribbon="Fork me on GitHub"
                        title="Fork me on GitHub"
                    >
                        Fork me on GitHub
                    </a>
                    <img src={logo} className="App-logo" alt="logo" />
                </header>
                {/* Render Root Directory Buttons */}
                {this.state.rootTree.map(directory => (
                    <div
                        onClick={this.filterSubDirectories}
                        id={directory.sha}
                        className="directory-button"
                        key={directory.sha}
                    >
                        {this.sanitizeWord(directory.name)}
                    </div>
                ))}
                {/* Render Sub Directories of Clicked Root Directory */}
                <div className="directory-choice-container">
                    {this.state.currentDirectory.map(
                        dir =>
                            !dir.path.includes('/') ? (
                                <a className="directory-choice" key={dir.sha} href={`#${dir.path}`}>
                                    <div>{this.sanitizeWord(dir.path)}</div>
                                </a>
                            ) : (
                                ''
                            ),
                    )}
                </div>
                {/* Render Files for Clicked Root Directory */}
                <div className="content-container">
                    {/* Renders Top Level SubDirectories i.e.  */}
                    {this.state.rootTree.map(
                        dir =>
                            !dir.name.includes('description') ? (
                                <div key={dir.path} className="content">
                                    <h3 className="content-header">{this.sanitizeWord(dir.name)}</h3>
                                    {dir.tree.map(subdir => {
                                        let subs = [];

                                        return !subdir.path.includes('/') ? (
                                            <div key={`${dir.path}/toplevel${subdir.path}`} className="sub-content">
                                                <h4 className="subcontent-header" id={subdir.path}>
                                                    {this.sanitizeWord(subdir.path)}
                                                </h4>
                                                {dir.tree.map(subsubdir => {
                                                    let subName = subsubdir.path.split('/');

                                                    if (
                                                        subsubdir.path.includes('/') &&
                                                        subName[0] === subdir.path &&
                                                        !subs.includes(subName[1]) &&
                                                        !subsubdir.path.includes('.png')
                                                    ) {
                                                        console.log(subName, !subs.includes(subName), subs);
                                                        subs.push(subName[1]);
                                                        return (
                                                            <div
                                                                className="language-sub-content"
                                                                key={`${dir.path}/${subName[1]}`}
                                                            >
                                                                <h5 className="language-subcontent-header">
                                                                    Language:{' '}
                                                                    <span className="language">
                                                                        {this.sanitizeWord(
                                                                            subsubdir.path.split('/')[1],
                                                                        )}
                                                                    </span>
                                                                </h5>
                                                                {dir.tree.map(
                                                                    files =>
                                                                        files.path.includes(
                                                                            subName[0] + '/' + subName[1],
                                                                        ) && files.path.includes('.') ? (
                                                                            files.path.includes('.png') ? (
                                                                                this.getContent(
                                                                                    `${dir.path}/${files.path}`,
                                                                                ) && (
                                                                                    <div
                                                                                        key={`${dir.path}/${files.path}`}
                                                                                    >
                                                                                        <h4 className="file-name-header">
                                                                                            File Name:{' '}
                                                                                            <span className="file-name">{files.path.split('/')[files.path.split('/').length - 1]}</span>
                                                                                        </h4>

                                                                                        <img
                                                                                            src={
                                                                                                this.state.codeContent[
                                                                                                    files.path
                                                                                                ]
                                                                                            }
                                                                                        />
                                                                                    </div>
                                                                                )
                                                                            ) : (
                                                                                <div key={`${dir.path}/${files.path}`}>
                                                                                    <h4 className="file-name-header">
                                                                                        File Name:{' '}
                                                                                        <span className="file-name">
                                                                                            {
                                                                                                files.path.split('/')[
                                                                                                    files.path.split(
                                                                                                        '/',
                                                                                                    ).length - 1
                                                                                                ]
                                                                                            }
                                                                                        </span>
                                                                                    </h4>
                                                                                    <button
                                                                                        className="view-code-button"
                                                                                        id={`${dir.path}/${files.path}`}
                                                                                        onClick={this.loadCode}
                                                                                    >
                                                                                        View Code
                                                                                    </button>
                                                                                    <div
                                                                                        style={{
                                                                                            position: 'relative',
                                                                                            whiteSpace: 'nowrap',
                                                                                            textAlign: 'left',
                                                                                        }}
                                                                                    >
                                                                                        <pre>
                                                                                            {
                                                                                                this.state.codeContent[
                                                                                                    files.path
                                                                                                ]
                                                                                            }
                                                                                        </pre>
                                                                                    </div>
                                                                                </div>
                                                                            )
                                                                        ) : (
                                                                            ''
                                                                        ),
                                                                )}
                                                            </div>
                                                        );
                                                    } else {
                                                        return '';
                                                    }
                                                })}
                                            </div>
                                        ) : (
                                            ''
                                        );
                                    })}
                                </div>
                            ) : (
                                // Descriptions
                                <div key={dir.path} className="content">
                                    <h3 className="content-header">{this.sanitizeWord(dir.name)}</h3>
                                    {dir.tree.map(subdir => {
                                        let subs = [];

                                        return !subdir.path.includes('/') ? (
                                            <div key={`${dir.path}/toplevel${subdir.path}`} className="sub-content">
                                                <h4 className="subcontent-header" id={subdir.path}>
                                                    {this.sanitizeWord(subdir.path.toLowerCase()).replace('.md', '')}
                                                </h4>

                                                {dir.tree.map(
                                                    files =>
                                                        files.path.includes(subdir.path) ? (
                                                            <div key={`${dir.path}/${files.path}`}>
                                                                <h4 className="file-name-header">
                                                                    File Name:{' '}
                                                                    <span className="file-name">
                                                                        {
                                                                            files.path.split('/')[
                                                                                files.path.split('/').length - 1
                                                                            ]
                                                                        }
                                                                    </span>
                                                                </h4>
                                                                <button
                                                                    className="view-code-button"
                                                                    id={`${dir.path}/${files.path}`}
                                                                    onClick={this.loadCode}
                                                                >
                                                                    View Code
                                                                </button>
                                                                <div
                                                                    style={{
                                                                        position: 'relative',
                                                                        whiteSpace: 'nowrap',
                                                                        textAlign: 'left',
                                                                    }}
                                                                >
                                                                    <div className="sanitized">
                                                                        <div
                                                                            dangerouslySetInnerHTML={this.setMarkDown(
                                                                                this.state.codeContent[files.path],
                                                                            )}
                                                                        />
                                                                    </div>
                                                                </div>
                                                            </div>
                                                        ) : (
                                                            ''
                                                        ),
                                                )}
                                            </div>
                                        ) : (
                                            ''
                                        );
                                    })}
                                </div>
                            ),
                    )}
                </div>

                {/* Display Code*/}
            </div>
        );
    }
}

export default App;
