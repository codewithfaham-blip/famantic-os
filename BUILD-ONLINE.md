# GitHub Codespaces Build Guide

## Step 1: Create GitHub Repository

1. Go to https://github.com/new
2. Create a new repository (name: `Famantic-OS`)
3. Make it **Public**

## Step 2: Upload Your Files

Upload all files from your `Famantic-os` folder to GitHub:
- Use GitHub's web interface "Add file" → "Upload files"
- Or use Git commands if you know how

## Step 3: Open in Codespaces

1. On your repository page, click the green **Code** button
2. Click **Codespaces** tab
3. Click **Create codespace on main**

This will open a full Linux environment in your browser!

## Step 4: Build the OS

In the Codespaces terminal (bottom panel), run:

```bash
# Install build tools (only needed once)
sudo apt update
sudo apt install build-essential nasm xorriso grub-pc-bin grub-common -y

# Build the OS
chmod +x build.sh
./build.sh
```

## Step 5: Download ISO

After build completes:
1. In the left sidebar, find `Famantic-os.iso`
2. Right-click → Download

Now you have your ISO file ready for VirtualBox!

---

**Note**: GitHub gives you 60 hours/month of Codespaces for FREE!
