const path = require("path");
const { merge } = require("webpack-merge");

const CopyWebpackPlugin = require("copy-webpack-plugin");
const HTMLWebpackPlugin = require("html-webpack-plugin");
const { CleanWebpackPlugin } = require("clean-webpack-plugin");
const MiniCssExtractPlugin = require("mini-css-extract-plugin");
const WorkboxWebpackPlugin = require("workbox-webpack-plugin");
const OptimizeCSSAssetsPlugin = require("optimize-css-assets-webpack-plugin");
const TerserPlugin = require("terser-webpack-plugin");
const HTMLInlineCSSWebpackPlugin = require("html-inline-css-webpack-plugin")
    .default;
const ImageminPlugin = require("imagemin-webpack-plugin").default;
// const BundleAnalyzerPlugin = require("webpack-bundle-analyzer")
//     .BundleAnalyzerPlugin;

const dist = path.join(__dirname, "dist/");
const mode =
    process.env.NODE_ENV === "production" ? "production" : "development";

const common = {
    mode,
    entry: "./src/Index.bs.js",
    output: {
        path: dist,
        publicPath: "/",
        filename: mode === "production" ? "[name]-[hash].js" : "index.js",
    },
    plugins: [
        new HTMLWebpackPlugin({
            template: "index.html",
            inject: "body",
            inlineSource: ".css$",
        }),
        // new BundleAnalyzerPlugin(),
    ],
    resolve: {
        modules: [path.join(__dirname, "src"), "node_modules"],
        extensions: [".bs.js", ".js", ".scss", ".css"],
        fallback: { stream: false },
    },
    module: {
        rules: [
            {
                test: /\.js$/,
                exclude: /node_modules/,
                use: {
                    loader: "babel-loader",
                },
            },
            {
                test: /\.scss$/,
                exclude: [/node_modules/],
                use: ["style-loader", "css-loader?url=false", "sass-loader"],
            },
            {
                test: /\.css$/,
                exclude: [],
                use: ["style-loader", "css-loader?url=false"],
            },
            {
                test: /\.woff(2)?(\?v=[0-9]\.[0-9]\.[0-9])?$/,
                exclude: [/node_modules/],
                type: "asset",
                mimetype: "application/font-woff",
                parser: {
                    dataUrlCondition: {
                        maxSize: 10 * 1024,
                    },
                },
            },
            {
                test: /\.(ttf|eot|svg)(\?v=[0-9]\.[0-9]\.[0-9])?$/,
                exclude: [/node_modules/],
                type: "asset",
                parser: {
                    dataUrlCondition: {
                        maxSize: 10 * 1024,
                    },
                },
            },
            {
                test: /\.(jpe?g|png|gif|svg)$/i,
                exclude: [/node_modules/],
                type: "asset",
                parser: {
                    dataUrlCondition: {
                        maxSize: 10 * 1024,
                    },
                },
            },
        ],
    },
};

if (mode === "development") {
    module.exports = merge(common, {
        devServer: {
            compress: true,
            contentBase: dist,
            port: process.env.PORT || 1234,
            historyApiFallback: true,
        },
    });
}
if (mode === "production") {
    module.exports = merge(common, {
        plugins: [
            new WorkboxWebpackPlugin.GenerateSW({
                swDest: dist + "sw.js",
                clientsClaim: true,
                skipWaiting: true,
            }),
            new HTMLInlineCSSWebpackPlugin(),
            new CleanWebpackPlugin({
                root: dist,
                exclude: [],
                verbose: false,
                dry: false,
            }),
            new CopyWebpackPlugin({
                patterns: [
                    {
                        from: "./assets",
                    },
                ],
            }),
            new ImageminPlugin({ test: /\.(jpe?g|png|gif|svg)$/i }),
            new MiniCssExtractPlugin({
                filename: "[name]-[hash].css",
                chunkFilename: "[id]-[contenthash].css",
            }),
            // new ShakePlugin(),
        ],
        module: {
            rules: [
                {
                    test: /\.css$/,
                    exclude: [],
                    use: [MiniCssExtractPlugin.loader, "css-loader?url=false"],
                },
                {
                    test: /\.scss$/,
                    exclude: [/node_modules/],
                    use: [
                        MiniCssExtractPlugin.loader,
                        "css-loader?url=false",
                        "sass-loader",
                    ],
                },
            ],
        },
        optimization: {
            splitChunks: {
                chunks: "async",
                cacheGroups: {
                    vendors: {
                        test: /[\\/]node_modules[\\/]/i,
                        chunks: "all",
                    },
                },
            },
            runtimeChunk: {
                name: "runtime",
            },
            minimize: true,
            minimizer: [
                new TerserPlugin({
                    test: /\.js$/i,
                    parallel: true,
                    terserOptions: {
                        compress: {
                            drop_console: true,
                            unsafe_comps: true,
                            unsafe: true,
                        },
                    },
                }),
                new OptimizeCSSAssetsPlugin({
                    cssProcessor: require("cssnano"),
                    cssProcessorPluginOptions: {
                        preset: [
                            "advanced",
                            {
                                discardComments: { removeAll: true },
                                cssDeclarationSorter: { order: "smacss" },
                            },
                        ],
                    },
                    canPrint: true,
                }),
            ],
        },
    });
}
