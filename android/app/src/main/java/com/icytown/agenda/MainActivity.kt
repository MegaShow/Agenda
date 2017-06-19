package com.icytown.agenda

import android.os.Bundle
import android.support.design.widget.FloatingActionButton
import android.support.design.widget.Snackbar
import android.view.View
import android.support.design.widget.NavigationView
import android.support.v4.view.GravityCompat
import android.support.v4.widget.DrawerLayout
import android.support.v7.app.ActionBarDrawerToggle
import android.support.v7.app.AppCompatActivity
import android.support.v7.widget.Toolbar
import android.view.Menu
import android.view.MenuItem
import android.content.Intent
import android.content.Context
import android.net.Uri
import java.net.HttpURLConnection
import java.net.URL
import android.R.id.edit
import android.content.SharedPreferences.Editor
import android.content.SharedPreferences
import android.support.annotation.Nullable
import android.widget.Button
import android.widget.EditText
import android.widget.TextView
import org.json.JSONObject
import android.os.AsyncTask.execute
import okhttp3.*
import java.io.IOException


class MainActivity : AppCompatActivity(), NavigationView.OnNavigationItemSelectedListener {

    public var cookie_name:String = ""
    public var cookie_id:Int = 0
    public var cookie_email:String = ""

    override fun onCreate(savedInstanceState: Bundle?) {
        val ctx = this@MainActivity
        val sp = ctx.getSharedPreferences("agenda", Context.MODE_PRIVATE)
        cookie_id = sp.getInt("cookie_id", 0)
        cookie_name = sp.getString("cookie_name", "")
        cookie_email = sp.getString("cookie_email", "")

        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        //setContentView(R.layout.signin_layout)
        val toolbar = findViewById(R.id.toolbar) as Toolbar
        setSupportActionBar(toolbar)

        val fab = findViewById(R.id.fab) as FloatingActionButton
        fab.setOnClickListener { view ->
            Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
                    .setAction("Action", null).show()
        }

        val drawer = findViewById(R.id.drawer_layout) as DrawerLayout
        val toggle = ActionBarDrawerToggle(
                this, drawer, toolbar, R.string.navigation_drawer_open, R.string.navigation_drawer_close)
        drawer.setDrawerListener(toggle)
        toggle.syncState()

        val navigationView = findViewById(R.id.nav_view) as NavigationView
        navigationView.setNavigationItemSelectedListener(this)

        /*val nameText = findViewById(R.id.NameTextView) as TextView
        val emailText = findViewById(R.id.EmailTextView) as TextView
        if (cookie_id != 0) {
            nameText.setText(cookie_name)
            emailText.setText(cookie_email)
        } else {
            nameText.setText("No sign in")
            emailText.setText("")
        }*/
    }

    override fun onBackPressed() {
        val drawer = findViewById(R.id.drawer_layout) as DrawerLayout
        if (drawer.isDrawerOpen(GravityCompat.START)) {
            drawer.closeDrawer(GravityCompat.START)
        } else {
            super.onBackPressed()
        }
    }

    override fun onCreateOptionsMenu(menu: Menu): Boolean {
        // Inflate the menu; this adds items to the action bar if it is present.
        val nameText = findViewById(R.id.NameTextView) as TextView
        val emailText = findViewById(R.id.EmailTextView) as TextView
        if (cookie_id != 0) {
            nameText.setText(cookie_name)
            emailText.setText(cookie_email)
        } else {
            nameText.setText("No sign in")
            emailText.setText("")
        }
        menuInflater.inflate(R.menu.main, menu)
        return true
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        val id = item.itemId


        if (id == R.id.action_settings) {
            return true
        }

        return super.onOptionsItemSelected(item)
    }

    override fun onNavigationItemSelected(item: MenuItem): Boolean {
        // Handle navigation view item clicks here.
        val id = item.itemId

        if (id == R.id.nav_mymeeting) {

        } else if (id == R.id.nav_setting) {

        } else if (id == R.id.nav_logout) {
            val ctx = this@MainActivity
            val sp = ctx.getSharedPreferences("agenda", Context.MODE_PRIVATE)
            val editor = sp.edit()
            editor.putInt("cookie_id", 0)
            editor.putString("cookie_name", "")
            editor.putString("cookie_email", "")
            editor.commit()
            cookie_id = 0
            cookie_name = ""
            cookie_email = ""
        } else if (id == R.id.nav_signin) {
            val editName = findViewById(R.id.editText) as EditText
            val editPassword = findViewById(R.id.editText2) as EditText
            val button = findViewById(R.id.button2) as Button
            editName.visibility = 1
            editPassword.visibility = 1
            button.visibility = 1
        } else if (id == R.id.nav_user) {

        } else if (id == R.id.nav_meeting) {

        }

        val drawer = findViewById(R.id.drawer_layout) as DrawerLayout
        drawer.closeDrawer(GravityCompat.START)
        return true
    }

    fun onClick(v: View?) {
        val uri = Uri.parse("https://github.com/FantasticGold/")
        val intent = Intent(Intent.ACTION_VIEW, uri)
        startActivity(intent)
    }

    fun onClickSignIn(v: View?) {
        val editName = findViewById(R.id.editText) as EditText
        val editPassword = findViewById(R.id.editText2) as EditText
        val url = "https://icytown.com/api/agenda/login/"
        val okHttpClient = OkHttpClient()
        val body = FormBody.Builder()
                .add("username", editName.getText().toString())
                .add("password", editPassword.getText().toString())
                .build()
        val request = Request.Builder()
                .url(url)
                .post(body)
                .build()
        val call = okHttpClient.newCall(request)
        call.enqueue(object : Callback {
            override fun onFailure(call: Call, e: IOException) {
                e.printStackTrace()
            }

            @Throws(IOException::class)
            override fun onResponse(call: Call, response: Response) {
                val res = response.body().string()
                val status = json.getStatus(res)
                if (status == "successful") {
                    cookie_id = json.getId(res)
                }
            }
        })
        val text = findViewById(R.id.textView3) as TextView
        text.setText(cookie_id.toString())
    }


}
